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

int xfreerdp_run(freerdp* instance)
{
	if (!freerdp_connect(instance))
		return 1;

    freerdp_disconnect(instance);
    return 0;
}

int main(int argc, char* argv[])
{
    int i             = 0;

    char *combinations[][4] = {
        // host, domain, user, cred
        { "192.168.56.101", NULL, "abc", "abc" },
        { "192.168.56.102", NULL, "ddd", "ddd" }
    };

    for (i = 0; i < sizeof (combinations) / sizeof (char*[4]); ++i)
    {
        freerdp* instance = freerdp_new();
        instance->PreConnect           = xf_pre_connect;
        instance->PostConnect          = xf_post_connect;
        instance->Authenticate         = xf_authenticate;
        instance->VerifyCertificate    = xf_verify_certificate;

        freerdp_context_new(instance);

        instance->settings->authentication_only = true;

        instance->settings->hostname = combinations[i][0];
        instance->settings->domain   = combinations[i][1];
        instance->settings->username = combinations[i][2];
        instance->settings->password = combinations[i][3];

        if (0 == xfreerdp_run (instance))
        {
            printf ("Host: %s, Domain: %s,  %s:%s\n", 
                    instance->settings->hostname,
                    instance->settings->domain,
                    instance->settings->username,
                    instance->settings->password);
        }
    
        freerdp_free(instance);
    }

	return 0;
}
