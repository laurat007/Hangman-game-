#include <stdio.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void desen(WINDOW *game, int i) 
		//in functie de nr de greseli transmis adauga elemente noi la spanzuratoare
{
	int j;
	if(i >= 1)
	{
		mvwprintw(game, 8, 53, "-----");
		mvwprintw(game, 9, 52, "|     |");
		mvwprintw(game, 10, 53, "-----");
		wrefresh(game);
	}
	if(i >= 2)
		for(j = 11; j <= 15; ++j)
		{
			mvwprintw(game, j, 55, "|"); 
			wrefresh(game);
		}
	if(i >= 3) 
	{
		mvwprintw(game, 13, 54, "\\");
		mvwprintw(game, 12, 53, "\\"); 
		wrefresh(game);
	}
	if(i >= 4)
	{
		mvwprintw(game, 13, 56, "/"); 
		mvwprintw(game, 12, 57, "/"); 
		wrefresh(game);
	}
	if(i >= 5)
	{
		mvwprintw(game, 16, 54, "/");
		mvwprintw(game, 17, 53, "/"); 
		wrefresh(game);
	}
	if(i == 6)
	{
		mvwprintw(game, 16, 56,"\\");
		mvwprintw(game, 17, 57,"\\");
		mvwprintw(game, 18, 52, "Game Over :(");
		mvwprintw(game, 19, 52, "Try Again :D"); 
		wrefresh(game);
	}
}
void entergame(WINDOW *game, int k, int car, char *ascuns, char *testate, int f)
{	int i, j;
	time_t t;
	struct tm *area;
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_BLUE);
	init_pair(3, COLOR_BLACK, COLOR_GREEN);
	init_pair(4, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(5, COLOR_BLACK, COLOR_WHITE);
	init_pair(6, COLOR_BLACK, COLOR_CYAN);
	srand(time(NULL));
	j = 1 + rand() % 6; 
	attron(COLOR_PAIR(j));
	wbkgd(game, COLOR_PAIR(j));
	wrefresh(game);
	t = time(NULL);
	area = localtime(&t);
	wclear(game);
	wrefresh(game);
		//panou de control
	mvwprintw(game, 2, 2, "%s\0",asctime(area)); //data
	wrefresh(game);
	mvwprintw(game, 5, 2, "%d expresii",k); 
	mvwprintw(game, 6, 2, "%d caractere",car);
	mvwprintw(game, 3, 2, "Q - back to menu");
	mvwprintw(game, 4, 2, "Insert character :D");
	mvwprintw(game, 3, 50, "%s",ascuns);
	mvwprintw(game, 20, 50, "%s",testate);
	wrefresh(game);
		//desenul propriu-zis al cadrului
	for(i = 5; i <= 20; ++i)
	{		
		wmove(game, i, 70); 
		waddch(game, '|'); 
		wmove(game, i, 71); 
		waddch(game, '|');
	}
	for(i = 50; i <= 71; ++i)
	{
		wmove(game, 5, i); 
		waddch(game, '-');
	}	
	wmove(game, 6, 55);
	waddch(game, '|');
	wmove(game, 7, 55);
	waddch(game, '|');
	for(i = 0; i < 4; ++i)
	{
		wmove(game, 17 + i, 69 - i);
		waddch(game, '/');
		wmove(game, 16 + i, 69 - i);
		waddch(game, '/');
	}	
	wmove(game, 20, 65);
	waddch(game, '/');
	for(i = 0 ; i < 31; ++i)
	{
		wmove(game, 1, i);
		waddch(game, '-');
		wmove(game, 7, i);
		waddch(game, '-');
	}
	for( i = 2; i <= 6; ++i)
	{
		wmove(game, i, 0);
		waddch(game, '|');
		wmove(game, i, 30);
		waddch(game, '|');
	}
	if(f == 1)
	{
		mvwprintw(game, 11, 2, "To start a new game press N\0");
		mvwprintw(game, 12, 2, "To go back to menu press Q\0");
		wrefresh(game);
	}
	attroff(COLOR_PAIR(j));
	wrefresh(game);
}

void print_menu(WINDOW *menu_win, int highlight)
{
	int x, y, i;	
	char *choices[] = { "New Game", "Resume Game", "Quit", };
	x = 2;
	y = 2;
	for(i = 0; i < 3; ++i)
	{	if(highlight == i + 1) //cauta optiunea selectata
		{	wattron(menu_win, A_REVERSE); 
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
			//highlight pe optiunea selectata
		}
		else
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		++y;
	}
	wrefresh(menu_win);
}

int main(int argc, char *argv[])
{	
	if(argc == 1) //verifica daca s-au dat argumente
	{
		printf("[Eroare]: Nu s-au dat argumente de comanda.\n"); 
		return 1;
	}
	else
	{
		WINDOW *menu_win, *game;
		FILE *argument;
		int highlight = 1, c, i, enable_res = 0, j = 0, final = 0;
		int k = 0, car = 0, rand_nr, l, lc, r, check = 0, g = 0;
		char **sir, ch, *ascuns, **cuvant, *copie, testate[50];
		ascuns = (char*)malloc(150 * sizeof(char));
		copie = (char*)malloc(sizeof(char));
		sir=(char**)malloc(400 * sizeof(char*));
		for(i = 0; i < 400; ++i)
			sir[i] = (char*)malloc(150 * sizeof(char));
		cuvant=(char**)malloc(500*sizeof(char*));
		for(i = 0; i < 500; ++i)
			cuvant[i]=(char*)malloc(150 * sizeof(char));
		int nfds = 1, sel, reall = 1;
			// nr de elemente din multimea read_descriptors  = nfds = 1
		struct timeval timeout;
		for(i = 1; i < argc; ++i)
		{
			argument = fopen(argv[i],"r"); //se deschide fiecare argument
			if (!argument) //testeaza daca fisierul poate fi deschis
			{
				printf("[Eroare]: Fisierul %s nu poate fi deschis.\n", argv[i]);
				return 1;
			}
			ch = fgetc(argument); //citeste primul caracter din fisier
			while(ch != EOF) //verific ca nu s-a terminat fisierul
			{
				if(ch != '\n')
				{
					if(isprint(ch))	
					{
						if((ch == ' ')&&(j!=0)&&(sir[k][j-1]==' '));
							//2 spatii consecutive nu sunt memorate
						else
						{
							ch = tolower(ch);
							sir[k][j] = ch;
							++j;
							++car;
						}
						//caracterele printabile sunt memorate in cate o expresie pana la '\n'
					}
				}
				else 
					if(j!=0) //liniile goale nu se memoreaza
					{
						++k; 
						j = 0;
					}
				if(k >= reall * 400)
				{		
					++reall;	
					sir=(char**)realloc(sir, k * 2 * sizeof(char*));
					for(i = 400 * (reall - 1); i < k * 2; ++i)
						sir[i] = (char*)realloc(sir[i], 150 * sizeof(char));
				}
				ch = fgetc(argument);
			}
		}			
		initscr();
		clear(); //se sterge ecranul
		noecho(); //se inhiba afisarea caracterelor introduse de la tastatura
		cbreak(); //caracterele introduse sunt citite imediat
		game = newwin(500, 500, 0, 0); //stabileste dimensiuni fereastra de joc
		menu_win = newwin(100, 100, 1, 1);
		keypad(menu_win, TRUE);
		refresh();
		print_menu(menu_win, highlight); // afiseaza meniu
		while(1)
		{	
			c=wgetch(menu_win); //primeste caracter de la tastatura
			if(c != 10)
			{
				switch(c)
				{	
					case KEY_UP:
						if(highlight == 1)
							highlight = 3;
						else
							--highlight;
						print_menu(menu_win, highlight);
						break;
					case KEY_DOWN:
						if(highlight == 3)
							highlight = 1;
						else 
							++highlight;
						print_menu(menu_win, highlight);
						break;
				}
				//cu ajutorul sagetilor sus-jos se selecteaza cu enter optiunea din meniu
			}
			else 
			{
				switch(highlight)
				{
					case 1: //a fost selectata optiunea New Game
						g = 0;
						final = 0;
						enable_res = 1;
							//optiune Resume Game devine activa
						testate[0] = '\0';
						srand(time(NULL));
						rand_nr = rand() % k; 
							//gemeraza numar intre 0 si nr total de expresii
						l = strlen(sir[rand_nr]);
						ascuns[0] = '\0';
						ch = '_';
						copie = strdup(sir[rand_nr]); 
							//lucram cu copie a sirului ales
						j = 0;
						for(i = 0; i < l; ++i)
							if(copie[i] == ' ')
							{
								strncpy(cuvant[j],copie,i+1);
								strcpy(copie,copie+i+1);  
								cuvant[j][i+1] = '\0';
								l = strlen(copie);
								i = 0; 
								lc = strlen(cuvant[j]); 
									//impartim expresia in cuvinte
								if((lc == 3) || (lc == 2)) 
									cuvant[j][lc - 2] = ch; 
								else 
									for(r = 1; r < lc - 2; ++r) 
										cuvant[j][r] = ch; 
									//"codificam" fiecare cuvant 						
								strcat(ascuns,cuvant[j]); 
									//adaugam cuvantul in sirul ascuns
								++j;
							}
						strcpy(cuvant[j],copie);
						cuvant[j][l] = '\0'; 
						if((l == 1) || (l == 2)) 
							cuvant[j][l - 1] = ch; 
						else 
							for(r = 1; r < l - 1; ++r) 
								cuvant[j][r] = ch;
						strcat(ascuns,cuvant[j]);
						l = strlen(sir[rand_nr]);
						for(i = 0; i < l; ++i) 
							for(j = 0; j < l; ++j) 
								if(ascuns[i] == sir[rand_nr][j])
									ascuns[j] = ascuns[i];
						ascuns[l] = '\0';
							//se procedeaza la fel pt ultimul cuvant
						while(1)
						{ 
							fd_set read_descriptors;
							FD_ZERO(&read_descriptors);
								//se curata multimea de lucru pt select
							FD_SET(0, &read_descriptors);
								//se adauga tastatura la multime
							timeout.tv_sec = 7;
								//specificam cate secunde vrem sa asteptam
							timeout.tv_usec = 0;
							entergame(game,k,car,ascuns,testate, final);
								//intram in fereastra de joc
							desen(game,g);
								//desenam elementele care indica etapa in care ne aflam
							sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
							switch (sel) 
							{
								case 1:
									c = wgetch(game);
									break;
								case 0:
									c = 32 + (random() % 95);
									while(c == 81)
										c = 32 + (random() % 95);
									break;
							}							
							FD_SET(0, &read_descriptors);
							timeout.tv_sec = 7;
							timeout.tv_usec = 0;
								//daca dupa 7 secunde nu sunt indroduse caractere, se genereaza unul aleator
							if(c == 81)
							{
								clear(); 
								refresh();
								print_menu(menu_win, highlight); 
								break;
							} //la apasarea tastei Q se revine la meniu
							else 
								if(isprint(c) && (c == tolower(c)))
								{
									for(i = 0; i < l; ++i)
										if(c == sir[rand_nr][i])
										{
											ascuns[i] = c; 
											check = 1;
										}
									//cauta caracter in sirul original
									if(check == 0) 
										for(i = 0; i < g; ++i) 
											if(c == testate[i]) 
												check = 1;
									//cauta caracterul in sirul de caractere testate deja, 
									//daca nu a fost gasit in sirul original
									if(check == 0)
									{ 
										testate[g] = c; 
										++g; 
										testate[g] = '\0';
									}	 
									//caracterele noi gresite sunt adaugate la caracterele testate
									check = 0;
									ascuns[l] = '\0';
									entergame(game, k, car, ascuns, testate, final);
									desen(game, g);
									//se actualizeaza desenul
									if(strcmp(ascuns,sir[rand_nr]) == 0) 
									{	
										mvwprintw(game, 10, 2, "Congratulation! You WON!\0");
										wrefresh(game);
									}
									//cand sirul ascuns si originalul coincid se afiseaza mesaj corespunzator
									if((strcmp(ascuns,sir[rand_nr]) == 0) || (g == 6))
									{
										final = 1;
										entergame(game, k, car, ascuns, testate, final);
										wrefresh(game);
										//la finalul jocului se afiseaza mesaj corespunzator
										enable_res = 0;
										g = 0;
										c = wgetch(game);
										//se citeste un nou caracter
										while((c != 81) && (c != 78))
											c = wgetch(game);
										if(c == 81)
										{
											clear(); 
											refresh();
											print_menu(menu_win, highlight);
											break;
										}
										//pentru Q, se revine la meniu
										if( c == 78)
										{
											final = 0;
											enable_res = 1;
											clear(); 
											refresh(); 
											wrefresh(game);
											srand(time(NULL));
											rand_nr = rand() % k;
											testate[0] = '\0';
											l = strlen(sir[rand_nr]);
											//pentru N, se genereaza un nou sir aleator
											ascuns[0] = '\0';
											ch = '_';
											copie = strdup(sir[rand_nr]);
											j = 0;
											for(i = 0; i < l; ++i)
												if(copie[i] == ' ')
												{
													strncpy(cuvant[j],copie,i + 1);
													strcpy(copie,copie + i + 1);  
													cuvant[j][i + 1] = '\0';
													l = strlen(copie);
													i = 0; 
													lc = strlen(cuvant[j]); 
													if((lc == 3) || (lc == 2)) 
														cuvant[j][lc - 2] = ch; 
													else 
														for(r = 1; r < lc - 2; ++r) 
													cuvant[j][r] = ch; 
													strcat(ascuns,cuvant[j]); 
													++j;
												}
											strcpy(cuvant[j],copie);
											cuvant[j][l] = '\0'; 
											if((l == 1)||(l == 2)) 
												cuvant[j][l - 1] = ch; 
											else 
												for(r = 1; r < l - 1; ++r) 
													cuvant[j][r] = ch;
											strcat(ascuns,cuvant[j]);
											l = strlen(sir[rand_nr]);
											for(i = 0; i < l; ++i) 
												for(j = 0; j < l; ++j) 
													if(ascuns[i] == sir[rand_nr][j])
														ascuns[j] = ascuns[i]; 
											ascuns[l] = '\0';
											entergame(game, k, car, ascuns, testate, final);
											desen(game,g);
										}
										// se repeta inceperea unui joc nou								
									}
								}
					}			
					break;
				case 2: // a fost selectata optiune Resume Game
					if(enable_res == 1) //daca este activa putem continua
					{		
						while(1)
						{	
							fd_set read_descriptors;
							FD_ZERO(&read_descriptors);
							FD_SET(0, &read_descriptors);
							timeout.tv_sec = 7;
							timeout.tv_usec = 0;
							entergame(game, k, car, ascuns, testate, final);
							desen(game, g);
							// afiseaza fereastra de joc in stadiul in care ramasese din optiunea New Game	
							sel = select(nfds, &read_descriptors, NULL, NULL, &timeout);
							switch (sel) 
							{
								case 1:
									c = wgetch(game);
									break;
								case 0:
									c = 32 + (random() % 95);
									while(c == 81)
										c = 32 + (random() % 95);
									break;
							}							
							FD_SET(0, &read_descriptors);
							timeout.tv_sec = 7;
							timeout.tv_usec = 0;
							//daca nu se da de la tastatura niciun caracter timp de 7 sec
							//se genereaza unul aleator
							if(c == 81) 
							{
								clear(); 
								refresh();
								print_menu(menu_win, highlight); 
								break;
							}
							//pentru Q, se revine la meniu
							else 
								if(isprint(c) &&  (c == tolower(c)))
								{
									for(i = 0; i < l; ++i)
										if(c == sir[rand_nr][i])
										{
											ascuns[i] = c; 
											check = 1;
										}
									if(check == 0) 
										for(i = 0; i < g; ++i) 
											if(c == testate[i]) 
												check = 1;
									if(check == 0)
									{
										testate[g] = c; 
										++g; 
										testate[g] = '\0';
									}	
										//se repeta cautarea caracterului
									check = 0;
									ascuns[l] = '\0';
									entergame(game, k, car, ascuns, testate, final);
									desen(game, g);
										//se actualizeaza desenul
									if(strcmp(ascuns,sir[rand_nr]) == 0) 
									{
										entergame(game, k, car, ascuns, testate, final);
										mvwprintw(game, 10, 2, "Congratulation! You WON!\0");
										wrefresh(game);
										desen(game, g);
									}								
										//daca ascuns si sirul original coincid se adauga mesajul corespunzator
									if((strcmp(ascuns,sir[rand_nr]) == 0) || (g == 6))
									{	
										final = 1;
										enable_res = 0;
										entergame(game, k, car, ascuns, testate, final);
										wrefresh(game);
										desen(game, g);
											//daca jocul s-a terminat se afiseaza noi instructiuni
										c = wgetch(game);
										while((c != 81) && (c != 78))
											c = wgetch(game);
										if(c == 81)
										{
											clear(); 
											refresh();
											print_menu(menu_win, highlight); 
											break;
										}
											//pentru Q se revine la meniu
										if( c == 78)
										{
											final = 0;
											clear(); 
											refresh(); 
											enable_res = 1;
											srand(time(NULL));
											rand_nr = rand() % k;
												//pentru N, se genereaza un nou sir aleator
											testate[0]='\0';
											g = 0;
											l = strlen(sir[rand_nr]);
											ascuns[0] = '\0';
											ch = '_';
											copie = strdup(sir[rand_nr]);
											j = 0;
											for(i = 0; i < l; ++i)
												if(copie[i] == ' ')
												{
													strncpy(cuvant[j],copie,i + 1);
													strcpy(copie,copie + i + 1);  
													cuvant[j][i + 1] = '\0';
													l = strlen(copie);
													i = 0; 
													lc = strlen(cuvant[j]); 
													if((lc == 3) || (lc == 2)) 
														cuvant[j][lc - 2] = ch; 
													else 
														for(r = 1; r < lc - 2; ++r) 
															cuvant[j][r] = ch; 
													strcat(ascuns,cuvant[j]); 
													++j;
												}
											strcpy(cuvant[j],copie);
											cuvant[j][l] = '\0'; 
											if((l == 1)||(l == 2)) 
												cuvant[j][l - 1] = ch; 
											else 
												for(r = 1; r < l - 1; ++r) 
													cuvant[j][r] = ch;
											strcat(ascuns,cuvant[j]);
											l = strlen(sir[rand_nr]);
											for(i = 0; i < l; ++i) 
												for(j = 0; j < l; ++j) 
											if(ascuns[i] == sir[rand_nr][j])
												ascuns[j] = ascuns[i]; 
											ascuns[l] = '\0';
											entergame(game,k,car,ascuns,testate, final);
											desen(game,g);
											//se repeta procedura pentru inceperea unui joc nou
										}	
									}
								}
							}
					}
					break;
				}
				if(highlight == 3) 
					break;
					//se selecteaza optiunea Quit
			}
		}
		refresh();
		endwin(); //se inchide fereastra ncurses
		free(ascuns);
		free(copie);
		return 0;
	}
}

