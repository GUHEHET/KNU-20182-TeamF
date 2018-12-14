total : ubunjam editor

ubunjam : ubunjam.c
	cc ubunjam.c -I/usr/local/include -L/usr/local/lib -lfmod -lpthread -lncursesw -o ubunjam

editor : editor.c
	cc editor.c -I/usr/local/include -L/usr/local/lib -lfmod -lpthread -lncursesw -o editor


