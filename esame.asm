CPU 8086
SECTION data
DaIndovinare: db 'In medio stat virtus', 0
Attuale: db '__ _____ ____ ______', 0
N: db 5
Fail: db 'Mi dispiace non hai indovinato!', 0ah, 0dh, 'La frase corretta era: ', 0
Success: db 'Congratulazioni! Hai indovinato!', 0
Trovato: db 0
ACapo: db 0ah, 0dh, 0
Tentativi: db 'Tentativi rimasti: ',0
TentativiSuccesso: db 'Hai indovinato in (n. tentativi): ', 0
Ntentativi: db 0

SECTION text
..start:
	mov ax, data
	mov ds, ax
	mov es, ax
	xor cx, cx
	mov cl, [N]
	
Inizio:
	inc byte [Ntentativi]
	mov byte [Trovato], 0; 		azzero il byte che utilizzo come flag per vedere se ho trovato il carattere inserito
	
	
	cmp byte [N], 0;			verifico che ci siano ancora tentativi disponibili					
	je FineFail;				se non ci sono ho fallito
	
	mov ax, Attuale
	push ax
	call StampaStringa
	add sp, 2
	
	mov ax, ACapo
	push ax
	call StampaStringa
	add sp, 2
	
	mov ax, Tentativi
	push ax
	call StampaStringa
	add sp, 2
	
	mov al, [N]
	add al, 30h
	mov ah, 0eh
	mov bx, 00h
	int 10h
	
	mov ax, ACapo
	push ax
	call StampaStringa
	add sp, 2
	
	mov si, Attuale
	
	;-------controllo se la stringa attuale sia completa, non puo succedere che non ho piu tentativi--------
	.controllovittoria:
	cmp byte [si], 0;			se sono arrivata alla fine della stringa vuol dire che non ci sono più '_': ho vinto
	je Vittoria
	cmp byte [si], '_';			appena trovo un '_' vuol dire che ho ancora caratteri da indovinare
	je .continua;				vado all'inserimento da tastiera del prossimo carattere
	inc si
	jmp .controllovittoria; 	se non ho trovato ne 0 ne '_' controllo il prossimo carattere
	
	.continua:
	mov ah, 00h;				leggo un carattere da tastiera		
	int 16h
	
	
	cmp al, 1bh;				se il carattere è ESC vado alla fine come se avessi fallito
	je FineFail
	
	xor ah, ah
	push ax;					bp+8 carattere letto
	mov ax, DaIndovinare
	push ax;					bp+6 DaIndovinare
	mov ax, Attuale
	push ax;					bp+4 Attuale
	
	
	call ControllaStringa;		altrimenti controllo se è presente nella stringa da indovinare
	add sp, 6
	cmp byte [Trovato], 1
	je Inizio
	
	dec byte [N]
	jmp Inizio
	
FineFail:
	mov ax, Fail
	push ax
	call StampaStringa
	add sp, 2
	
	mov ax, DaIndovinare
	push ax
	call StampaStringa
	add sp, 2

	jmp Fine
	
Vittoria:
	mov ax, Success
	push ax
	call StampaStringa
	add sp, 2
	
	mov ax, ACapo
	push ax
	call StampaStringa
	add sp, 2
	
	mov ax, TentativiSuccesso
	push ax
	call StampaStringa
	add sp, 2
	
	xor ax, ax
	xor dx, dx
	mov al, [Ntentativi]
	mov dl, 10h
	div dl 
	mov dh, ah
	add al, 30h
	mov ah, 0eh
	mov bx, 00h
	int 10h
	mov al, dh 
	add al, 30h
	mov ah, 0eh
	mov bx, 00h
	int 10h
	
Fine:
	mov ax, 4c00h
	int 21h
	
;----funzioni----
	
ControllaStringa:
	push bp
	mov bp, sp
	
	mov si, [bp+6]
	mov di, [bp+4]
	mov al, [bp+8]
	xor bx, bx
	mov bx, [bp+6]

	
	.iniziocontrollo:;			controllo se sono arrivato alla fine della stringa
	cmp byte [si], 0
	je .finecontrollo
	cmp [si], al;				controllo se il carattere inserito è uguale al carattere attuale della stringa DaIndovinare
	je .trovato;				se sono uguali lo vado a inserire sulla stringa Attuale
	inc si
	jmp .iniziocontrollo;		altrimenti passo ad analizzare il prossimo carattere della stringa DaIndovinare
	
	.trovato:
	xchg bx, si
	sub bx, si;					bx ora contiene la posizione assoluta del carattere nella stringa
	mov [di+bx], al
	xchg bx, si
	add si, bx;					ripristino la posizione di si
	mov byte [Trovato], 1;		questo flag a 1 indica che ho trovato la lettera inserita almeno una volta nella stringa DaIndovinare
	inc si
	jmp .iniziocontrollo

	.finecontrollo:
	pop bp
	ret
	
	
	
StampaStringa:;					funzione generica per stampare una stringa precedentemente inserita nello stack
	push bp
	mov bp, sp
	mov si, [bp+4]
	.stampastringa:
	lodsb 
	cmp al, 0
	je .finestampa
	
	mov ah, 0eh
	mov bx, 00h
	int 10h
	jmp .stampastringa

	.finestampa:
	pop bp
	ret