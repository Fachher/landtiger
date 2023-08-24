#JLinkGDBServer -device LPC1768 -If SWD -CommanderScript flashlpc1768.jlink


JLinkGDBServer -select USB 683193986 -device LPC1768 -speed 400 -if SWD -port 2331 -singlerun
