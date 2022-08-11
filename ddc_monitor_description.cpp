// ddc_monitor_description.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <vector>
#include <lowlevelmonitorconfigurationapi.h>
#include <windowsx.h>

#pragma comment(lib, "Dxva2.lib")

using namespace std;

struct MonitorDesc
{
    HANDLE hdl;
    DWORD power;
};

// Monitor enumeration callback
BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
    std::vector<MonitorDesc>* pMonitors = reinterpret_cast<std::vector<MonitorDesc>*>(dwData);

    DWORD nMonitorCount;
    if (GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &nMonitorCount))
    {
        PHYSICAL_MONITOR* pMons = new PHYSICAL_MONITOR[nMonitorCount];

        if (GetPhysicalMonitorsFromHMONITOR(hMonitor, nMonitorCount, pMons))
        {
            for (DWORD i = 0; i < nMonitorCount; i++)
            {
                MonitorDesc desc;
                desc.hdl = pMons[i].hPhysicalMonitor;

                pMonitors->push_back(desc);
            }
        }
        delete[] pMons;
    }
    return TRUE;
}

LPSTR GetMonitorCapabilityString(HANDLE hPhysicalMonitor)
{
    DWORD cchStringLength = 0;
    BOOL bSuccess = 0;
    LPSTR szCapabilitiesString = NULL;

    bSuccess = GetCapabilitiesStringLength(hPhysicalMonitor, &cchStringLength);

    if (bSuccess)
    {
        // Allocate the string buffer.
        LPSTR szCapabilitiesString = (LPSTR)malloc(cchStringLength);
        if (szCapabilitiesString != NULL)
        {
            // Get the capabilities string.
            bSuccess = CapabilitiesRequestAndCapabilitiesReply(hPhysicalMonitor, szCapabilitiesString, cchStringLength);

            return szCapabilitiesString;
        }
    }
    return NULL;
}

BOOL PrintVCPCodeValues(HANDLE hPhysicalMonitor, BYTE vcpCode)
{
    DWORD currentValue;
    DWORD maximumValue;
    MC_VCP_CODE_TYPE codeType;
    BOOL bCommandSuccess;

    cout << " >>> Printing information for VCP code 0x" << hex << (int)vcpCode << dec << endl;

    bCommandSuccess = GetVCPFeatureAndVCPFeatureReply(hPhysicalMonitor, vcpCode, &codeType, &currentValue, &maximumValue);

    if (bCommandSuccess)
    {
        cout << "VCP: 0x" << hex << (int)vcpCode << dec << endl;
        cout << "Value: 0x" << hex << currentValue << " (0d" << dec << currentValue << ")" << endl;
        cout << "MaximumValue: 0x" << hex << maximumValue << " (0d" << dec << maximumValue << ")" << endl;

        cout << "VCP CodeType: ";

        switch (codeType) {
        case MC_MOMENTARY:
            cout << "MC_MOMENTARY";
            break;
        case MC_SET_PARAMETER:
            cout << "MC_SET_PARAMETER";
            break;
        }

        cout << endl;
    } 
    else
    {
        cout << " <<< Command Error" << endl;
    }


    return bCommandSuccess;
}

#define VCP_IMAGEADJUSTMENTS_SHARPNESS 0x87
#define VCP_DISPLAYCONTROL_VCPVERSION 0xDF
#define VCP_DISPLAYCONTROL_DISPLAYCONTROLLER_ID 0xC8
#define VCP_MISC_INPUTSOURCE 0x60


#define VCP_VALUE_MISC_INPUTSOURCE_VGA1 0x01
#define VCP_VALUE_MISC_INPUTSOURCE_VGA2 0x02
#define VCP_VALUE_MISC_INPUTSOURCE_DVI1 0x03
#define VCP_VALUE_MISC_INPUTSOURCE_DVI2 0x04
#define VCP_VALUE_MISC_INPUTSOURCE_COMPOSITE1 0x05
#define VCP_VALUE_MISC_INPUTSOURCE_COMPOSITE2 0x06
#define VCP_VALUE_MISC_INPUTSOURCE_SVIDEO1 0x07
#define VCP_VALUE_MISC_INPUTSOURCE_SVIDEO2 0x08
#define VCP_VALUE_MISC_INPUTSOURCE_TUNER1 0x09
#define VCP_VALUE_MISC_INPUTSOURCE_TUNER2 0x0A
#define VCP_VALUE_MISC_INPUTSOURCE_TUNER3 0x0B
#define VCP_VALUE_MISC_INPUTSOURCE_COMPONENT1 0x0C
#define VCP_VALUE_MISC_INPUTSOURCE_COMPONENT2 0x0D
#define VCP_VALUE_MISC_INPUTSOURCE_COMPONENT3 0x0E
#define VCP_VALUE_MISC_INPUTSOURCE_DP1 0x0f
#define VCP_VALUE_MISC_INPUTSOURCE_DP2 0x10
#define VCP_VALUE_MISC_INPUTSOURCE_HDMI1 0x11
#define VCP_VALUE_MISC_INPUTSOURCE_HDMI2 0x12

LPSTR DisplayInputSourceFriendlyName(DWORD displayInputCode)
{
    switch (displayInputCode)
    {
    case VCP_VALUE_MISC_INPUTSOURCE_VGA1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_VGA1");
    case VCP_VALUE_MISC_INPUTSOURCE_VGA2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_VGA2");
    case VCP_VALUE_MISC_INPUTSOURCE_DVI1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_DVI1");
    case VCP_VALUE_MISC_INPUTSOURCE_DVI2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_DVI2");
    case VCP_VALUE_MISC_INPUTSOURCE_COMPOSITE1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_COMPOSITE1");
    case VCP_VALUE_MISC_INPUTSOURCE_COMPOSITE2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_COMPOSITE2");
    case VCP_VALUE_MISC_INPUTSOURCE_SVIDEO1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_SVIDEO1");
    case VCP_VALUE_MISC_INPUTSOURCE_SVIDEO2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_SVIDEO2");
    case VCP_VALUE_MISC_INPUTSOURCE_TUNER1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_TUNER1");
    case VCP_VALUE_MISC_INPUTSOURCE_TUNER2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_TUNER2");
    case VCP_VALUE_MISC_INPUTSOURCE_TUNER3: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_TUNER3");
    case VCP_VALUE_MISC_INPUTSOURCE_COMPONENT1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_COMPONENT1");
    case VCP_VALUE_MISC_INPUTSOURCE_COMPONENT2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_COMPONENT2");
    case VCP_VALUE_MISC_INPUTSOURCE_COMPONENT3: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_COMPONENT3");
    case VCP_VALUE_MISC_INPUTSOURCE_DP1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_DP1");
    case VCP_VALUE_MISC_INPUTSOURCE_DP2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_DP2");
    case VCP_VALUE_MISC_INPUTSOURCE_HDMI1: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_HDMI1");
    case VCP_VALUE_MISC_INPUTSOURCE_HDMI2: return const_cast<LPSTR>("VCP_VALUE_MISC_INPUTSOURCE_HDMI2");
    default: return const_cast<LPSTR>("RESERVED/UNASSIGNED/UNKNOWN");
    }
}

BOOL GetDisplayInputSource(HANDLE hPhysicalMonitor, LPDWORD displayInputCode)
{
    DWORD currentValue;
    DWORD maximumValue;
    MC_VCP_CODE_TYPE codeType;
    BOOL bCommandSuccess;

    return GetVCPFeatureAndVCPFeatureReply(hPhysicalMonitor, VCP_MISC_INPUTSOURCE, &codeType, displayInputCode, &maximumValue);
}

BOOL SetDisplayInputSource(HANDLE hPhysicalMonitor, DWORD displayInputSourceCode)
{
    MC_VCP_CODE_TYPE codeType;
    BOOL bCommandSuccess;


    return SetVCPFeature(hPhysicalMonitor, VCP_MISC_INPUTSOURCE, displayInputSourceCode);
}


int main()
{
    cout << "\t DDC/CI MCCS Analyser\n" << endl;

 


    std::vector<MonitorDesc> monitors;
    if (EnumDisplayMonitors(NULL, NULL, &MonitorEnumProc, reinterpret_cast<LPARAM>(&monitors)))
    {
        int i = 1;

        for (auto& monitor : monitors)
        {
            DWORD target = 0x00120000 | VCP_VALUE_MISC_INPUTSOURCE_HDMI1;
            cout << "\n\t == Monitor " << i++ << " ==" << endl;
            PrintVCPCodeValues(monitor.hdl, VCP_MISC_INPUTSOURCE);
            SetDisplayInputSource(monitor.hdl, target);
            PrintVCPCodeValues(monitor.hdl, VCP_MISC_INPUTSOURCE);
            
            //SetVCPFeature(monitor.hdl, VCP_MISC_INPUTSOURCE, VCP_VALUE_MISC_INPUTSOURCE_HDMI1);
            // PrintVCPCodeValues(monitor.hdl, 0x52);


            // LPSTR caps = GetMonitorCapabilityString(monitor.hdl);

            // DWORD displayInput;

            //if (GetDisplayInputSource(monitor.hdl, &displayInput))
            //{
            //    cout << "InputMode: " << DisplayInputSourceFriendlyName(displayInput) << endl;
            // }

            // SetVCPFeature(monitor.hdl, 0xD6, 0x01);
            // SetVCPFeature(monitor.hdl, VCP_MISC_INPUTSOURCE, VCP_VALUE_MISC_INPUTSOURCE_DP1);

            //cout << " >>> Trying to switch to input source " << DisplayInputSourceFriendlyName(VCP_VALUE_MISC_INPUTSOURCE_HDMI1) << endl;

            //if (SetDisplayInputSource(monitor.hdl, VCP_VALUE_MISC_INPUTSOURCE_HDMI1))
            // {
            //    cout << " <<< VCP reported back OK" << endl;
            // }
        }
    }
    else 
    {
        cout << "Fail to aquire monitor handles";
    }    
}