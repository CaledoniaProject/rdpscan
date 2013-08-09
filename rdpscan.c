#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/select.h>
#include <freerdp/constants.h>

#include <freerdp/freerdp.h>

boolean xf_pre_connect(freerdp* instance)
{
    return true;
}

boolean xf_post_connect(freerdp* instance)
{
    return true;
}

boolean xf_authenticate(freerdp* instance, char** username, char** password, char** domain)
{
	return true;
}

boolean xf_verify_certificate(freerdp* instance, char* subject, char* issuer, char* fingerprint)
{
    return true;
}

void xf_run(char *hostname, char *user, char *pass, char *domain)
{
    freerdp* instance = freerdp_new();
    instance->PreConnect           = xf_pre_connect;
    instance->PostConnect          = xf_post_connect;
    instance->Authenticate         = xf_authenticate;
    instance->VerifyCertificate    = xf_verify_certificate;

    freerdp_context_new(instance);

    instance->settings->hostname = hostname;
    instance->settings->domain   = domain;
    instance->settings->username = user;
    instance->settings->password = pass;

	if (freerdp_connect(instance))
    {
        printf ("SUCCESSFUL LOGIN - %s - %s\\%s : %s\n",
                instance->settings->hostname,
                instance->settings->domain,
                instance->settings->username,
                instance->settings->password);
    
        freerdp_disconnect(instance);
    }

    freerdp_free(instance);
}

int main(int argc, char* argv[])
{
    switch (argc)
    {
        case 4:
            xf_run (argv[1], argv[2], argv[3], argv[4]);
            break;
        case 3:
            xf_run (argv[1], argv[2], argv[3], NULL);
            break;
        default:
            printf ("Usage:\n\t%s hostname username credential [domain]\n", argv[0]);
            return 1;
    }

    return 0;
}
