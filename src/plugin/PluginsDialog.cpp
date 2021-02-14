#include "PythonPlugin.h"

#include "PluginsDialog.h"
#include "ui_PluginsDialog.h"

PluginsDialog::PluginsDialog(PythonPluginInterface* pyPlugin, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PluginsDialog),
    pluginSet_(Persistent<PluginSet>( "pluginSet" )),
    pythonPlugin_(pyPlugin)
{
    ui->setupUi(this);

    ui->pluginSystemEnabled->setChecked(pluginSet_->isPluginSystemEnabled());
    createPluginClassList();
}

PluginsDialog::~PluginsDialog()
{
    delete ui;
}

void PluginsDialog::createPluginClassList()
{
    for(const auto& type: pythonPlugin_->getConfig()){
        QListWidgetItem* item = new QListWidgetItem(type.first.c_str(), ui->listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(type.second ? Qt::Checked : Qt::Unchecked); // AND initialize check state
    }

    enableItemChangeHandler = true;
}

void PluginsDialog::on_buttonBox_accepted()
{
    pluginSet_->setPlugins(pythonPlugin_->getConfig());
    pluginSet_->setPluginSystemEnabled(pythonPlugin_->isEnabled());
    GetPersistentInfo().save("pluginSet");
}

void PluginsDialog::on_pluginSystemEnabled_clicked(bool checked)
{
    pythonPlugin_->enable(checked);
}

void PluginsDialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
//    bool checked = item->checkState();
//    QString text = item->text();
//    emit pluginsOptionsChanged(text.toStdString(), checked);
}

void PluginsDialog::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{

}

void PluginsDialog::on_listWidget_itemChanged(QListWidgetItem *item)
{
    if(enableItemChangeHandler){
        bool checked = item->checkState();
        QString text = item->text();
        cout << text.toStdString() << " " << checked << "\n";
        emit pluginsOptionsChanged(text.toStdString(), checked);
    }
}