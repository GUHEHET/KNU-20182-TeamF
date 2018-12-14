total : ubunjam ubunjam_mac

ubunjam : ubunjam.c
	cc ubunjam.c -I/usr/local/include -L/usr/local/lib -lfmod -lpthread -lncursesw -o ubunjam

ubunjam_mac : ubunjam_mac.c
	cc ubunjam_mac.c -I/usr/local/include -L/usr/local/lib -lfmod -lpthread -lncurses -o ubunjam_mac


