#include <stdlib.h>
#include <stdio.h>
#include "GfaschannelTest.h"
#include "GfaschannelUserDefinedTest.h"

/**
 * @brief
 *
 * @param handle -- lib handle, ret. by @e DaEnableAccess() call
 * @param lun    -- logical unit number.
 *
 * @return
 */
int UserDefinedMenu(HANDLE handle, int lun)
{
  int i;
  int choice;
  int do_usr_wait = 0;

  for (;;) {
	  printf("%sGFASCHANNEL %s <V. %d> Test Program - User"
		 " Defined Menu\n[%s]\n",
		 ClearScreen, (lun < 0)?"Simulator":"Driver", g_drvrVers,
		 g_drvrVersInfo);

	  for (i = 0; i < screenWidth(); i++)
		  printf("-");

	  printf("\n\n");
	  printf("01 - Return to Main Menu\n");

	  /*====================================================================
	   * INSERT YOUR MENU CODE HERE. (see GfaschannelTest.c for more
	   * details)
	   * example:
	   * printf("02 - Do something\n");
	   * printf("03 - Do something\n");
	   * printf("04 - Do something\n");
	   *......................................
	   * printf("nn - Do something\n");
	   * etc...
	   *====================================================================
	   */

	  printf("\n> ");

	  scanf("%d", &choice);
	  getchar();

	  switch (choice) {
	  case 1:
		  do_usr_wait = 0;
		  return(OK);
		  break;

		  /*============================================================
		   * INSERT YOUR MENU CODE HERE. (see GfaschannelTest.c for
		   * more details)
		   * example:
		   * case 2:
		   * case 3:
		   * case 4:
		   *......................................
		   * case n:
		   * etc..
		   *============================================================
		   */
	  default:
		  printf("Please enter a valid menu item.\n\n<enter>"
			 " to continue");
		  getchar();
		  break;
	  }

	  if (do_usr_wait) {
		  printf("\n<enter> to continue");
		  getchar();
	  }
  }
}
