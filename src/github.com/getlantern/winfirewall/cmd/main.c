/*
 * Test utility for Windows Firewall COM interface library
 */

#include "winfirewall.h"

int main(int argc, wchar_t* argv[])
{
        HRESULT hr = S_OK;
        HRESULT com_init = E_FAIL;
        INetFwProfile* fw_profile = NULL;

        // Initialize COM.
        com_init = CoInitializeEx(0, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        // Ignore RPC_E_CHANGED_MODE; this just means that COM has already been
        // initialized with a different mode. Since we don't care what the mode is,
        // we'll just use the existing mode.
        if (com_init != RPC_E_CHANGED_MODE) {
                hr = com_init;
                if (FAILED(hr)) {
                        printf("CoInitializeEx failed: 0x%08lx\n", hr);
                        goto error;
                }
        }

        // Retrieve the firewall profile currently in effect.
        hr = windows_firewall_initialize(&fw_profile);
        if (FAILED(hr)) {
                printf("Firewall failed to initialize: 0x%08lx\n", hr);
                goto error;
        }

        // Check Firewall status
        BOOL is_on;
        hr = windows_firewall_is_on(fw_profile, &is_on);
        if (FAILED(hr)) {
            printf("Error retrieving Firewall status: 0x%08lx\n", hr);
            goto error;
        }
        if (is_on) {
            printf("Windows Firewall is ON\n");
        } else {
            printf("Windows Firewall is OFF\n");
        }

error:
        // Release the firewall profile.
        windows_firewall_cleanup(fw_profile);

        // Uninitialize COM.
        if (SUCCEEDED(com_init)) {
                CoUninitialize();
        }

        return 0;
}