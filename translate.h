#include "Common.h"

#ifndef _TRANSLATE_H_
#define _TRANSLATE_H_

#if TARGET == PC

char EnglishSetupLangArray[][1000] = 
{
"Install",
"Complete install",
"Play game",
"Video / Sound options",
"Controller options",
"Uninstall",
"Exit game",
"Video options",
"Video device",
"Game resolution",
"Sound device",
"Sound options",
"Player one configuration",
"Player two configuration",
"Controller",
"Redefine keys",
"Run 'Game Options' Panel",								// 16

"Please select a control device",
"You have not selected a controller for player one.\n\nClick 'Ok' and complete your selection.", 
"Please click 'ok' and complete your selection",
"Keyboard configuration",
"Action",												// 21
"Key",													
"Up",
"Down",
"Left",
"Right",
"Attack",												// 27
"Jump",
"Block",
"Brake",
"Throw",
"You have not defined keys for all actions.\n\nPlease press 'Ok' and complete your selection.",
"Define",

"Joystick configuration",								// 34
"Axis",
"Up / Down",
"Left / Right",
"Button",												// 38
"You have not defined a control for all actions",
"You have not defined keys for all the actions for player one.\n\nPlease press 'Ok' and select 'Redefine Keys'.", 

"Ok",													// 41
"Cancel",
"Retry",
			
"Backspace",
"Tab",	
"Return",
"Left Ctrl",
"Right Ctrl",
"Left Shift",
"Right Shift",
"Left Alt",
"Space",	
"Caps Lock",
					
"F1",
"F2",
"F3",
"F4",
"F5",
"F6",
"F7",
"F8",
"F9",
"F10",
"F11",
"F12",
					
"Num lock",
"Scroll lock",
"Numpad",
"System Request",

"Right Alt",
"Pause",
"Home",
"Cursor Up",
"Page Up",		
"Cursor Left",
"Cursor Right",
"End",	
"Cursor Down",
"Page Down",
"Insert",
"Delete",		
"Num Lock",
"Please insert the CD and try again."
};


char FrenchSetupLangArray[][1000] = 
{
"Installation",							// 0
"Installation termin?e",
"Jouer",
"Options Vid?o / Son",					// 3
"Options manette",
"D?sinstaller",
"Quitter partie",
"Options Vid?o",
"P?riph?rique vid?o",
"R?solution du jeu",
"P?riph?rique son",
"Options Son",
"Configuration joueur un",
"Configuration joueur deux",
"Manette",
"Red?finir touches",
"Ex?cuter Panneau ?Options de jeu?",	// 16

"Veuillez s?lectionner un p?riph?rique de commande",
"Vous n'avez pas s?lectionn? de manette pour le joueur un.\n\nCliquez sur ?Ok? et terminez votre s?lection.",
"Veuillez cliquer sur ?Ok? et terminer votre s?lection",
"Configuration du clavier",
"Action",								// 21
"Touche",
"Haut",
"Bas",
"Gauche",
"Droite",
"Attaquer",								// 27
"Sauter",
"Bloquer",
"Freiner",
"Lancer",
"Vous n'avez pas d?fini toutes les touches des actions.\n\nVeuillez appuyer sur ?Ok? et terminer votre s?lection.",
"D?finir",
"Configuration de la manette",			// 34
"Axe",
"Haut / Bas",
"Gauche / Droite",
"Touche",								// 38!!!!
"Vous n'avez pas d?fini toutes les commandes pour les actions",
"Vous n'avez pas d?fini toutes les touches pour les actions du joueur un.\n\nVeuillez appuyer sur ?Ok? et s?lectionner ?Red?finir touches?.",
"Ok",
"Annuler",
"R?essayer",
"Retour arri?re",
"Tabulation",
"Validation",
"Ctrl gauche",
"Ctrl droite",
"MAJ gauche",
"MAJ droite",
"Alt gauche",
"Espace",
"Touche de verrouillage majuscules",
"F1",
"F2",
"F3",
"F4",
"F5",
"F6",
"F7", 
"F8",
"F9",
"F10",
"F11",
"F12",
"Ver. num",
"D?fil",
"Pav? num?rique",
"Requ?te du syst?me",
"Alt droit",
"Pause",
"D?but",
"Curseur haut",
"Pg. Suiv",
"Curseur gauche",
"Curseur droite",
"Fin",			
"Curseur bas",
"Pg. Pr?c",
"Insertion",
"Suppr",
"Ver. num",
"Veuillez ins?rer le CD et essayez encore."
};


char GermanSetupLangArray[][1000] = 
{
"Installieren",
"Komplette Installation",
"Spiel starten",
"Video / Sound Optionen",
"Controller Optionen",
"Deinstalieren",
"Exit",
"Video Optionen",
"Grafikkarte",
"Aufl?sung",
"Soundkarte",
"Sound Optionen",
"Konfiguration Spieler 1",
"Konfiguration Spieler 2",
"Controller",
"Tastenbelegung",
"Fenster 'Spieloptionen' ?ffnen",
"Bitte Steuerung w?hlen",
"F?r Spieler 1 wurde kein Controllr ausgew?hlt.\n\nKlicken Sie auf 'OK' und beenden Sie Ihre Auswahl",
"Bitte klicken Sie auf 'OK' und beenden Sie Ihre Auswahl",
"Tastatur Konfiguration",
"Aktion",
"Taste",
"Oben",
"Unten",
"Links",
"Rechts",
"Angriff",
"Springen",
"Blockieren",
"Bremsen",
"Werfen",
"Sie haben nicht jede Aktion mit einer Taste belegt.\n\nBitte dr?cken Sie 'OK' und beenden Sie Ihre Auswahl",
"Definieren",
"Joystick Konfiguration",
"Achse",
"Oben / Unten",
"Links / Rechts",
"Taste",
"Sie haben nicht f?r alle Aktionen die Steuerung definiert",
"Sie haben f?r Spieler 1 nicht allen Aktionen eine Taste zugewiesen.\n\nBitte klicken Sie auf 'OK' und w?hlen Sie  'Tastenbelegung'.",
"Ok",
"Abbrechen",
"Noch mal versuchen",
"Backspace",
"Tab",
"Return",
"Strg links",
"Strg rechts",
"Shift links",
"Shift rechts",
"Alt links",
"Space",
"Caps Lock",
"F1",
"F2",
"F3",
"F4",
"F5",
"F6",
"F7",
"F8",
"F9",
"F10",
"F11",
"F12",
"Zahlenblock sperren",
"Scroll lock",
"Zahlenblock",
"System Request",
"Alt rechts",
"Pause",
"Home",
"Cursor nach oben",
"Bild aufw?rts",
"Cursor links",
"Cursor rechts",
"Ende",
"Cursor nach unten",
"Bild abw?rts",
"Einlegen",
"L?schen",
"Zahlenblock sperren",
"Setzen Sie bitte das CD ein und versuchen Sie noch einmal"
};


char SpanishSetupLangArray[][1000] = 
{
"Instalar",
"Completar instalaci?n",
"Jugar partida",
"Opciones de v?deo y sonido",
"Opciones del controlador",
"Desinstalar",
"Dejar la partida",
"Opciones de v?deo",
"Dispositivo de v?deo",
"Resoluci?n del juego",
"Dispositivo de sonido",
"Opciones de sonido",
"Configuraci?n del jugador uno",
"Configuraci?n del jugador dos",
"Controlador",
"Redefinir teclas",
"Ejecutar el panel de 'Opciones del juego'",
"Escoge un dispositivo de control",
"No has escogido un controlador para el jugador uno.\n\nHaz clic en 'Aceptar' y realiza tu selecci?n.",
"Haz clic en 'Aceptar' y realiza tu selecci?n",
"Configuraci?n del teclado",
"Acci?n",
"Tecla",
"Arriba",
"Abajo",
"Izquierda",
"Derecha",
"Atacar",
"Saltar",
"Bloquear",
"Frenar",
"Lanzar",
"No has definido teclas para todas las acciones.\n\nHaz clic en 'Aceptar' y realiza tu selecci?n.",
"Definir",
"Configuraci?n del joystick",
"Eje",
"Arriba / Abajo",
"Izquierda / Derecha",
"Bot?n",
"No has definido controles para todas las acciones",
"No has definido teclas para todas las acciones del jugador uno.\n\nHaz clic en 'Aceptar' y selecciona 'Redefinir teclas'.",
"Aceptar",
"Cancelar",
"Volver a intentar",
"Retroceso",
"Tabulador",
"Retorno",
"Control izqda.",
"Control dcha.",
"May?sculas izqda.",
"May?sculas dcha.",
"Alt izqda.",
"Espacio",
"Bloq. May?s.",
"F1",
"F2",
"F3",
"F4",
"F5",
"F6",
"F7",
"F8",
"F9",
"F10",
"F11",
"F12",
"Bloq. Num.",
"Bloq desplazamiento",
"Teclado num?rico",
"Solicitud del sistema",
"Alt dcha.",
"Pausa",
"Inicio",
"Cursor arriba",
"Re P?g",
"Cursor izqda.",
"Cursor dcha.",
"Fin",
"Cursor abajo",
"Av P?g",
"Insertar",
"Eliminar",
"Bloq. Num.",
"Inserte por favor el CD y procure otra vez."
};


char ItalianSetupLangArray[][1000] = 
{
"Installa",
"Completa Installazione",
"Gioca",
"Opzioni audio/video",
"Opzioni controller",
"Disinstalla",
"Esci dal gioco",
"Opzioni video",
"Dispositivo video",
"Risoluzione gioco",
"Dispositivo sonoro",
"Opzioni Suono",
"Configurazione giocatore 1",
"Configurazione giocatore 2",
"Controller",
"Ridefinisci i tasti",
"Lancia quadro \"Opzioni gioco\"",
"Selezionare un dispositivo di  controllo",
"Non avete selezionato alcun controller per il giocatore 1.\n\nfate clic su \"ok\" e completate la vostra selezione",
"Fate clic su \"ok\" e completate la vostra selezione",
"Configurazione tastiera",
"Azione",
"Tasto",
"S?",
"Gi?",
"Sinistra",
"Destra",
"Attaccare",
"Saltare",
"Bloccare",
"Frenare",
"Lanciare",
"Non avete definito i tasti per tutte le azioni.\n\nPremete \"ok\" e completate la selezione",
"Definire",
"Configurazione stick",
"Asse",
"S?/Gi?",
"Sinistra/Destra",
"Tasto",
"Non avete definito un controllo per tutte le azioni",
"Non avete definito i tasti per tutte le azioni del giocatore 1.\n\nPremete \"ok\" e selezionate \"Ridefinire tasti\".",
"Ok",
"Annulla",
"Riprova",
"indietro",
"Tab",
"Invio",
"Crtl Sinistro",
"Ctrl Destro",
"Shift Sinistro",
"Shift Destro",
"Alt Sinistro",
"Spazio",
"BlocMaiusc",
"F1",
"F2",
"F3",
"F4",
"F5",
"F6",
"F7",
"F8",
"F9",
"F10",
"F11",
"F12",
"Bloc Num",
"Bloc Scorr",
"NumPad",
"Requisito di sistema",
"Alt destro",
"Pausa",
"Home",
"Cursore s?",
"Pagina s?",
"Cursore a sinistra",
"Cursore a destra",
"Fine",
"Cursore gi?",
"Pagina gi?",
"Inserire",
"Cancellare",
"Bloc Num",
"Inserire prego il CD e provare ancora"
};

#endif
#endif