#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"

int main (int argc, char **argv)
{
   FILE *screenshot_command;
   char screenshot_command_response[COMMAND_RESPONSE];
   char choice[MAX_INPUT_LENGTH];
   char first_character_in_choice;
   int input_length;
   int shadow_state = ENABLED;

   if (NULL == (screenshot_command = popen(
         "defaults read com.apple.screencapture disable-shadow", "r"))
   ) {
      printf("\n\t\t%sUnable to access the required screen shot command.%s\n",
         BOLDRED, BLACK);

      exit(EXIT_FAILURE);

   } else {
      while (NULL != fgets(
               screenshot_command_response,
               sizeof(screenshot_command_response) - 1,
               screenshot_command)
      ) {
         ;
      }
   }

instructions:
   switch(screenshot_command_response[0]) {
      case '0':
         shadow_state = ENABLED;

         printf("\n\n\tThe box shadow option for screenshots is currently enabled.\n"
            "\tPress %sy%s to continue shadowing screenshots, %sn%s to disable it,\n"
            "\tor %sq%s to quit.\n\n",
            BOLDRED, BLACK,
            BOLDRED, BLACK,
            BOLDRED, BLACK);

         break;

      case '1':
         shadow_state = DISABLED;

         printf("\n\n\tThe box shadow option for screenshots is currently disabled.\n"
            "\tPress %sn%s to keep it disabled, %sy%s to enable it, or %sq%s\n"
            "\tto quit.\n\n",
            BOLDRED, BLACK,
            BOLDRED, BLACK,
            BOLDRED, BLACK);

         break;

      default:
         printf("\n\n\tEither this is the first time you’ve run this program, or\n"
            "there is a problem with the command. Resetting the default…\n\n");

         if (NULL == (screenshot_command = popen(
               "defaults write com.apple.screencapture disable-shadow -bool false",
               "r"))
         ) {
            printf( "Unable to access the required screen shot command.\n" );

            exit(EXIT_FAILURE);

         } else {
            while (NULL != fgets(
               screenshot_command_response,
               sizeof(screenshot_command_response) - 1, screenshot_command)
            ) {
               ;
            }
         }

         goto instructions;
   }

   printf("\t%sNote: Once you make a choice, a subsytem of Mac OS X called\n"
          "\tthe SystemUIServer will restart, likely launching iTunes. This\n"
          "\tis normal.\n\n%s",
          BLACK, BLACK);

user_choice:
   scanf("%s", choice);
   input_length = strlen(choice);

   if (input_length > 1) {
      printf("\n\t%sInput is too long.%s Please try again...\n\n",
             BOLDRED, BLACK);

      goto user_choice;
   }

   if (1 == input_length) {
      first_character_in_choice = choice[0];

      switch (first_character_in_choice) {
         case 'y':
            if (ENABLED == shadow_state) {
               printf("\n\tNothing was changed, because you’ve asked to keep "
                  "shadowing screenshots.\n\n");
            } else {
               if (DISABLED == shadow_state) {
                  system("defaults write com.apple.screencapture disable-shadow -bool false");
                  system("killall SystemUIServer");
               }
            }

            break;

         case 'n':
            if (ENABLED == shadow_state) {
               system("defaults write com.apple.screencapture disable-shadow -bool true");
               system("killall SystemUIServer");
            } else {
               if (DISABLED == shadow_state) {
                  printf("\n\tNothing was changed, because you’ve asked to continue "
                     "disabling shadows around screenshots.\n\n");
               }
            }

            break;

         case 'q':
         case 'Q':
            printf("\t%sQuitting%s\n\n",
               BOLDRED, BLACK);

            exit(0);

            break;

         default:
            printf("\n\t%sInvalid character.%s Try again. "
                   "(Enter 'q' or 'Q' to quit.)\n\n",
               BOLDRED, BLACK);

            goto user_choice;
      }
   }
   else {
      printf("\n\t%sInvalid input.%s Try again. "
             "(Enter 'q' or 'Q' to quit.)\n\n",
         BOLDRED, BLACK);

      goto user_choice;
   }

   pclose(screenshot_command);

   return 0;
}
