#include "domain_monitor_thread.h"

DomainMonitorThread::DomainMonitorThread(
        QObject *parent, virConnectPtr* connPtr, QString _domainName) :
    QThread(parent), currConnPtr(connPtr), domainName(_domainName)
{
    prev_cpuTime = 0;
    firstStep = true;
    tMark.start();
}
DomainMonitorThread::~DomainMonitorThread()
{
    // release the reference because no longer required
    if ( currConnPtr!=NULL ) {
        if ( NULL!=domain ) {
            virDomainFree(domain);
        };
        int ret = virConnectClose(*currConnPtr);
        //qDebug()<<"virConnectRef -1"<<"DomainStateViewer"<<domainName<<(ret+1>0);
        // for reject the multiple releasing the reference
        currConnPtr = NULL;
    };
    wait(30000);
}

/* public slots */

/* private slots */
void DomainMonitorThread::run()
{
    if ( firstStep ) {
        // for new virConnect usage create the new virConnectRef[erence]
        int ret = virConnectRef(*currConnPtr);
        if ( ret<0 ) currConnPtr = NULL;
        //qDebug()<<"virConnectRef +1"<<"DomainMonitorThread"<<domainName<<(ret+1>0);
        domain = virDomainLookupByName(
                    *currConnPtr, domainName.toUtf8().data());
    };
    if ( NULL!=domain ) {
        virDomainInfo info;
        quint64 curr_cpuTime, cpu_time_diff, _time_diff,
                CPU_percent, MEM_percent, MEM;
        if ( virDomainGetInfo(domain, &info)+1 ) {
            curr_cpuTime = info.cpuTime;
            _time_diff = tMark.elapsed()/1000;
            cpu_time_diff = (firstStep)? 0 : curr_cpuTime - prev_cpuTime;
            CPU_percent = (qreal)100/(_time_diff * info.nrVirtCpu)*(qreal)cpu_time_diff/1000000000;
            tMark.restart();
            MEM = info.memory;
            MEM_percent = 100*(qreal)MEM / info.maxMem;
            prev_cpuTime = curr_cpuTime;
            if ( firstStep ) firstStep = false;
        };
        if ( info.state == VIR_DOMAIN_RUNNING || info.state == VIR_DOMAIN_PAUSED ) {
            emit dataChanged(info.state, CPU_percent, MEM_percent, MEM);
        } else
            emit dataChanged(info.state, 0, 0, 0);
    } else {
        firstStep = true;
    };
}
