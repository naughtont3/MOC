#include <moc.h>
#include <stdio.h>
#include <stdlib.h>

int
main (int argc, char **argv)
{
    int rc;

    rc = MOC_Init (NULL);
    if (rc != MOC_SUCCESS)
    {
        fprintf (stderr, "ERROR: MOC_Init() failed\n");
        return EXIT_FAILURE;
    }

    rc = MOC_Fini ();
    if (rc != MOC_SUCCESS)
    {
        fprintf (stderr, "ERROR: MOC_Fini() failed\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
