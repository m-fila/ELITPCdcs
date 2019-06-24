#include "lvpsucontroller.h"
#include <iostream>

LVpsuController::LVpsuController() : ReadoutPeriod(100)
{

}
/*
void LVpsuController::run()
{
    stopLoop = false;
    isStopped = false;
    int ctr = 0;

    string response = device->getIdentifier();

    emit statusChanged(QString::fromStdString(response));

    emit settingsReadFromDevice(getSettings());

    Command c;
    while(!stopLoop)
    {
        if(!commandQueue.isEmpty())
        {
            ctr=0;

            c = commandQueue.dequeue();
            if(!c.name.compare("OUTP:SEL"))
            {
                device->setOutputSel(c.parameters.at("CHno").toInt(), c.parameters.at("CHsel").toInt());
                this->msleep(10);
                emit measurementsReadFromDevice(getMeasurements());
            }
            else if(!c.name.compare("OUTP:GEN"))
            {
                device->setOutputGen(c.parameters.at("GEN").toInt());
                this->msleep(10);
                emit measurementsReadFromDevice(getMeasurements());
            }

        }
        else
        {
            if(ctr<ReadoutPeriod)
                ctr++;
            else
            {
                ctr=0;
                emit measurementsReadFromDevice(getMeasurements());
            }
        }
        this->msleep(10);
    }
    isStopped = true;
}
*/
void LVpsuController::setCH1outputON()
{
    Command cmd;
    cmd.name = "OUTP:SEL";
    cmd.parameters["CHno"] = 1;
    cmd.parameters["CHsel"] = true;
    commandQueue.enqueue(cmd);
}

void LVpsuController::setCH1outputOFF()
{
    Command cmd;
    cmd.name = "OUTP:SEL";
    cmd.parameters["CHno"] = 1;
    cmd.parameters["CHsel"] = false;
    commandQueue.enqueue(cmd);
}

void LVpsuController::setCH2outputON()
{
    Command cmd;
    cmd.name = "OUTP:SEL";
    cmd.parameters["CHno"] = 2;
    cmd.parameters["CHsel"] = true;
    commandQueue.enqueue(cmd);
}

void LVpsuController::setCH2outputOFF()
{
    Command cmd;
    cmd.name = "OUTP:SEL";
    cmd.parameters["CHno"] = 2;
    cmd.parameters["CHsel"] = false;
    commandQueue.enqueue(cmd);
}

void LVpsuController::setGENoutputON()
{
    Command cmd;
    cmd.name = "OUTP:GEN";
    cmd.parameters["GEN"] = true;
    commandQueue.enqueue(cmd);
}

void LVpsuController::setGENoutputOFF()
{
    Command cmd;
    cmd.name = "OUTP:GEN";
    cmd.parameters["GEN"] = false;
    commandQueue.enqueue(cmd);
}

HMPseriesSettings LVpsuController::getSettings()
{
    HMPseriesSettings hmpSettings;
    string response;

    device->setActiveChannel(1);
    hmpSettings.CH1_voltage = device->getVoltageSet();
    hmpSettings.CH1_current = device->getCurrentSet();
    response = device->getOutputSel();
    hmpSettings.CH1 = (response=="1") ? true : false;

    device->setActiveChannel(2);
    hmpSettings.CH2_voltage = device->getVoltageSet();
    hmpSettings.CH2_current = device->getCurrentSet();
    response = device->getOutputSel();
    hmpSettings.CH2 = (response=="1") ? true : false;

    // general output
    response = device->getOutputGen();
    hmpSettings.Output = (response=="1") ? true : false;

    return hmpSettings;
}

HMPseriesMeasurements LVpsuController::getMeasurements()
{
    HMPseriesMeasurements hmpMeasurements;
    string response;

    device->setActiveChannel(1);
    hmpMeasurements.CH1_voltage = device->getVoltage();
    hmpMeasurements.CH1_current = device->getCurrent();
    response = device->getOutputSel();
    hmpMeasurements.CH1 = (response=="1") ? true : false;

    device->setActiveChannel(2);
    hmpMeasurements.CH2_voltage = device->getVoltage();
    hmpMeasurements.CH2_current = device->getCurrent();
    response = device->getOutputSel();
    hmpMeasurements.CH2 = (response=="1") ? true : false;

    // general output
    response = device->getOutputGen();
    hmpMeasurements.Output = (response=="1") ? true : false;

    return hmpMeasurements;
}
