#include "iscsi_sec_type.h"

/*
 * http://libvirt.org/formatsecret.html#iSCSIUsageType
 */

iSCSISecType::iSCSISecType(QWidget *parent, virConnectPtr _conn) :
    _SecType(parent, _conn)
{
    usage = new QLineEdit(this);
    usage->setPlaceholderText("Enter usage name");
    usageLayout = new QHBoxLayout();
    usageLayout->addWidget(usage);
    usageWdg = new QWidget(this);
    usageWdg->setLayout(usageLayout);
    baseLayout->addWidget(usageWdg);
}
QDomDocument iSCSISecType::getSecStuff() const
{
    /*
     * <usage type='iscsi'>
            <target>libvirtiscsi</target>
       </usage>
     */
    QDomDocument doc;
    QDomElement _usage, _target;
    QDomText    _text = doc.createTextNode(usage->text());
    _usage = doc.createElement("usage");
    _usage.setAttribute("type", "iscsi");
    doc.appendChild(_usage);
    _target = doc.createElement("target");
    _target.appendChild(_text);
    _usage.appendChild(_target);
    //qDebug()<<doc.toByteArray(4).data();
    return doc;
}