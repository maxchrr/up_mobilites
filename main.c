/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include "api.c"
#include "utils.h"

int main(void)
{
    struct Bus_Stop  **sts;
    struct Bus_Route **rts;
    struct Bus_Line  **bls;
    int n_sts, n_rts, n_bls;
    int i;

    if (load_data("x.data", &sts, &n_sts, &rts, &n_rts, &bls, &n_bls) == 0)
    {
        /* affichage */
        for (i = 0; i < n_bls; i++)
            print_bl(bls[i], 0);

        /* sauvegarde avant libération */
        save_data("x.data", sts, n_sts, rts, n_rts, bls, n_bls);

        /* libération de la mémoire */
        for (i = 0; i < n_bls; i++) free_bl(bls[i]);
        for (i = 0; i < n_rts; i++) free_br(rts[i]);
        for (i = 0; i < n_sts; i++) free_bs(sts[i]);
        free(bls);
        free(rts);
        free(sts);

        return 0;
    }

    /* fallback : création manuelle si x.data introuvable */
    struct Bus_Stop* bs1 = create_bs(1, "Foo", 10, 20);
    struct Bus_Stop* bs2 = create_bs(2, "Bar", 20, 20);
    struct Bus_Route* br = create_br(1, bs1, bs2, 1, 1);
    struct Bus_Line* bl = create_bl(br);
    print_bl(bl, 0);
    free_bl(bl);
    free_br(br);
    free_bs(bs1);
    free_bs(bs2);
    return 0;
}
