#include "domain_control_menu.h"

DomainControlMenu::DomainControlMenu(
        QWidget *parent, QStringList params, bool state) :
    QMenu(parent), parameters(params), autoReloadState(state)
{
    if ( !parameters.isEmpty() ) {
        start = new QAction("Start", this);
        start->setIcon(QIcon::fromTheme("start"));
        start->setVisible(parameters.last()=="yes" && parameters[1]!="active" );
        pause = new QAction("Pause", this);
        pause->setIcon(QIcon::fromTheme("pause"));
        pause->setVisible(parameters[1]=="active");
        destroy = new QAction("Destroy", this);
        destroy->setIcon(QIcon::fromTheme("destroy"));
        destroy->setVisible(parameters[1]=="active");
        edit = new QAction("Edit", this);
        edit->setIcon(QIcon::fromTheme("configure"));
        edit->setVisible(true);
        reset = new QAction("Reset", this);
        reset->setIcon(QIcon::fromTheme("reset"));
        reset->setVisible(parameters[1]=="active");
        reboot = new QAction("Reboot", this);
        reboot->setIcon(QIcon::fromTheme("reboot"));
        reboot->setVisible(parameters[1]=="active");
        shutdown = new QAction("Shutdown", this);
        shutdown->setIcon(QIcon::fromTheme("shutdown"));
        shutdown->setVisible(parameters[1]=="active");
        save = new QAction("Save", this);
        save->setIcon(QIcon::fromTheme("save"));
        save->setVisible(parameters[1]=="active");
        undefine = new QAction("Undefine", this);
        undefine->setIcon(QIcon::fromTheme("undefine"));
        undefine->setVisible(parameters.last()=="yes");
        autoStart = new QAction("change AutoStart", this);
        autoStart->setIcon(QIcon::fromTheme("autostart"));
        autoStart->setVisible(parameters.last()=="yes");
        createSnapshot = new QAction("Snapshot now!", this);
        createSnapshot->setIcon(QIcon::fromTheme("camera-photo"));
        moreSnapshot_Actions = new QAction("more Snapshot actions", this);
        moreSnapshot_Actions->setIcon(QIcon::fromTheme("camera-photo"));
        getXMLDesc = new QAction("get XML Description", this);
        getXMLDesc->setIcon(QIcon::fromTheme("application-xml"));
        getXMLDesc->setVisible(true);
        RunningData = new QAction("current state data", this);
        InactiveData = new QAction("inactive state data", this);
        xmlDescParams = new QMenu(this);
        xmlDescParams->addAction(RunningData);
        xmlDescParams->addAction(InactiveData);
        getXMLDesc->setMenu(xmlDescParams);
        display = new QAction("display VM", this);
        display->setIcon(QIcon::fromTheme("display"));
        display->setVisible(parameters[1]=="active");
        displayInExternal = new QAction("display VM in external Viewer", this);
        displayInExternal->setIcon(QIcon::fromTheme("display"));
        displayInExternal->setVisible(parameters[1]=="active");
        addToMonitor = new QAction("add to State Monitor", this);
        addToMonitor->setIcon(QIcon::fromTheme("utilities-monitor"));
        addToMonitor->setVisible(true);
        migrate = new QAction("Migrate", this);
        migrate->setIcon(QIcon::fromTheme("migrate"));
        migrate->setVisible(parameters[1]=="active");

        addAction(start);
        addAction(pause);
        addAction(destroy);
        addAction(undefine);
        addAction(autoStart);
        addSeparator();
        addAction(edit);
        addSeparator();
        addAction(createSnapshot);
        addAction(moreSnapshot_Actions);
        addSeparator();
        addAction(display);
        addAction(displayInExternal);
        addAction(addToMonitor);
        addSeparator();
        addAction(migrate);
        addSeparator();
        addAction(getXMLDesc);
        addSeparator();
        addAction(reset);
        addAction(reboot);
        addAction(shutdown);
        addAction(save);
        addSeparator();
    };
    reload = new QAction("Reload Domain OverView", this);
    reload->setIcon(QIcon::fromTheme("view-refresh"));
    reload->setVisible(!autoReloadState);
    setSeparatorsCollapsible(true);

    addAction(reload);
    connect(this, SIGNAL(triggered(QAction*)),
            this, SLOT(emitExecMethod(QAction*)));
}

/* private slots */
void DomainControlMenu::emitExecMethod(QAction *action)
{
    Act_Param paramList;
    if ( action == start) {
        paramList.method = startEntity;
    } else if ( action == pause ) {
        paramList.method = pauseEntity;
    } else if ( action == destroy ) {
        paramList.method = destroyEntity;
    } else if ( action == edit ) {
        paramList.method = editEntity;
    } else if ( action == reset ) {
        paramList.method = resetVirtDomain;
    } else if ( action == reboot ) {
        paramList.method = rebootVirtDomain;
    } else if ( action == shutdown ) {
        paramList.method = shutdownVirtDomain;
    } else if ( action == save ) {
        paramList.method = saveVirtDomain;
    } else if ( action == undefine ) {
        paramList.method = undefineEntity;
    } else if ( action == autoStart ) {
        paramList.method = setAutostartEntity;
        paramList.path =
               (QString((parameters[2]=="yes")? "0" : "1"));
    } else if ( action == RunningData ) {
        paramList.method = getEntityXMLDesc;
        paramList.context = DO_AsIs;
    } else if ( action == InactiveData ) {
        paramList.method = getEntityXMLDesc;
    } else if ( action == display ) {
        paramList.method = displayVirtDomain;
    } else if ( action == displayInExternal ) {
        paramList.method = displayVirtDomainInExternalViewer;
    } else if ( action == addToMonitor ) {
        paramList.method = monitorVirtDomain;
    } else if ( action == migrate ) {
        paramList.method = migrateVirtDomain;
    } else if ( action == reload ) {
        paramList.method = reloadEntity;
    } else if ( action == createSnapshot ) {
        paramList.method = createVirtDomainSnapshot;
    } else if ( action == moreSnapshot_Actions ) {
        paramList.method = moreSnapshotActions;
    } else return;
    //qDebug()<<paramList<<"paramList from menu";
    emit execMethod(paramList);
}
