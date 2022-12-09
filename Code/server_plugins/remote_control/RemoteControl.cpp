
#include "RemoteControl.h"
#include "Pch.h"

// This is a great resource that i used while embedding python:
// https://pybind11.readthedocs.io/en/stable/advanced/embedding.html
RemoteControlPlugin::~RemoteControlPlugin()
{
}

bool RemoteControlPlugin::Initialize()
{

    PluginAPI_WriteLog(LogLevel::kInfo, "Initialized remote control plugin");
    return true;
}

void RemoteControlPlugin::Shutdown()
{
    PluginAPI_WriteLog(LogLevel::kInfo, "Shutting down remote control plugin");
}

void RemoteControlPlugin::OnEvent(uint32_t aEventCode)
{
}

void RemoteControlPlugin::OnTick()
{
}