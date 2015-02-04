#include "main.h"

void UASRTinterrupt();
extern volatile uint8_t MODE;

//char EEMEM EEMEMpassword[16];


int main()
{
	//eeprom_write_block("test", (void *)492, 7);

	LowLevelDevice.init();
	Interface.init();
	Login.refreshPassword();
	
	sei();
	
	Interface.putstring_P(PSTR("Kanciapa Zamek initialized\n\r"));
	
	CardBase.read_base_from_EEPROM();
	//CardBase.list_base();
	
	char * now_card = Buffer.receive_data(); //pointer is constant, so...
	
	while(1)
	{
		MODE = 1;
		while(1)
		{
			if( MODE == 0 )
				break;
			while ( ! Buffer.new_data_available() )
			{
				if( MODE == 0 )
					goto Computer_MODE;
			}
			Authenticator.process_card(now_card);
			Buffer.clear_buffer();
		}

		Computer_MODE:
		
		char buf[20], now;
		uint8_t it = 0;
		char * xx, *yy;
		
		
		Interface.putstring_P(PSTR("--- Zamek do Knaciapy Control Center ---\n\rpassword: "));
		if ( !Login.askForPassword() )
		{
			continue;
		}
		while(1)
		{
			
			Interface.putstring_P(PSTR("root@zamek / # "));

			xx = Interface.readline();

			if( (!strncmp("help", xx,4)) || (!strncmp("?",xx,1)))//  xx[0] == 'h' || xx[0] == '?' )
			{
				Interface.putstring_P(PSTR("\n\rWelcome to Zamek do Kanciapy Control Center!\n\rcommand list:\n\radd - adding user,\n\rrm - deleting user,\n\rls - list users,\n\rclear - clear user base\n\ropen - open the door\n\r\n\rAuthor: Grzegorz Gajoch, gajoch@gmail.com, IX 2013\n\r"));
			}
			else if( !strncmp("add",xx,3) )//xx[0] == 'a' )
			{
				Interface.putstring_P(PSTR("\n\rID of card to insert: "));
				//add card
				//read card
				
				char * b = Interface.readline();
				
				Person tmp(b);
				if( CardBase.authorised_card(tmp) )
				{
					Interface.putstring_P(PSTR("\n\rCard already in database!\n\r"));
				}
				else
				{
					CardBase.add_card(tmp);
					Interface.putstring_P(PSTR("\n\rInserted card with ID "));
					for(uint8_t i = 0; i < 10; ++i) Interface.putch(b[i]);
					Interface.putstring_P(PSTR(" and hash "));
					tmp.getID(buf);
					//itoa(tmp.Hash, buf, 10);
					//sprintf(buf,"%ld\n\r",tmp.Hash);
					Interface.putstring(buf);
				}
				free(b);
			}
			else if( (!strncmp("del",xx,3)) || (!strncmp("rm",xx,3)) )
			{
				Interface.putstring_P(PSTR("\n\rID of card to delete: "));
				//delete card
				char * b = Interface.readline();
				Person tmp(b);
				
				if( CardBase.authorised_card(tmp) )
				{
					CardBase.delete_card(tmp);
					
					Interface.putstring_P(PSTR("\n\rDeleted card with ID "));
					for(uint8_t i = 0; i < 10; ++i) Interface.putch(b[i]);
					Interface.putstring_P(PSTR(" and hash "));
					tmp.getID(buf);
					//itoa(tmp.Hash, buf, 10);
					//sprintf(buf,"%ld\n\r",tmp.Hash);
					Interface.putstring(buf);
				}
				else
				{
					Interface.putstring_P(PSTR("\n\rNo such card in database!\n\r"));
				}
				free(b);
			}
			else if( !strncmp("ls",xx,2) )
			{
			//show list
				Interface.putstring_P(PSTR("\n\r"));
				CardBase.list_base();
			}
			else if( !strncmp("clear",xx,5) )
			{
			//clear list
				Interface.putstring_P(PSTR("\n\rAre You sure You want to destroy all database? (Y/N) "));
				char * b = Interface.readline();
				if( b[0] == 'Y' || b[0] == 'y' )
				{
					CardBase.clean_base();
				}
				Interface.putstring_P(PSTR("\n\r"));
				free(b);
			}
			else if( !strncmp("exit",xx,4) )
			{
				Interface.putstring_P(PSTR("\n\r\n\r"));
				break;
			}
			else if( !strncmp("passwd",xx,6) )
			{
				Interface.putstring_P(PSTR("\n\rOld password: "));
				if( !Login.askForPassword() )
				{
					Interface.putstring_P(PSTR("Incorrect password\n\rThe password is unchanged.\n\r"));
				}
				else
				{
					Interface.putstring_P(PSTR("New password: "));
					char * new_password_pointer = Interface.readline(false);
					uint8_t it = 0;
					while(  new_password_pointer[it] )
					{
						Login.SRAMpassword[it] =  new_password_pointer[it];
						++it;
					}
					Login.SRAMpassword[it] =  new_password_pointer[it];
					free( new_password_pointer);
					
					Login.writeToEEPROM(it+1);
					Interface.putstring_P(PSTR("\n\rPassword changed.\n\r"));
				}
			}
			else if( !strncmp("open",xx,4) )
			{
				Interface.putstring_P(PSTR("\n\rDoor opened!\n\r"));
				LowLevelDevice.open_door();
			}
			else if( xx[0] == 0 ) {}
			else
			{
				Interface.putstring_P(PSTR("\n\rShell: command not found: "));
				Interface.putstring(xx);
				Interface.putstring_P(PSTR("\n\r"));
			}
			free(xx);
			
		}
	}
		

	//read command
	//run command
	//show OK

}
