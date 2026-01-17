#include "instruction.h"

short pop (Programme* programme){ 
    short x = programme->donnees[programme->PC-1];
    if (x < 0 || x > 4999){
        printf("pop : Adresse non valide\n");
        return 1;
    }
    programme->SP -= 1;
    if (programme->SP < 0){
        printf("pop : erreur, il n'y a pas d'adresse en memoire\n");
        return 1;
    }
    programme->memoire[x] = programme->memoire[programme->SP];
    return 0;
}


short ipop (Programme* programme){
    if (programme->SP-2 < 0){
        printf("ipop : erreur, il n'y a pas assez de valeurs en memoire\n");
        return 1;
    }
    programme->SP -= 1;
    short n = programme->memoire[programme->SP];
    if (n < 0 || n > 4999){
        printf("ipop : Adresse non valide\n");
        return 1;
    }
    programme->SP -= 1;
    programme->memoire[n]=programme->memoire[programme->SP];
    return 0;
}


short push (Programme* programme){
    short x = programme->donnees[programme->PC-1];
    if (x < 0 || x > 4999){
        printf("push : Adresse non valide\n");
        return 1;
    }
    if (programme->SP > 4999){
    	printf ("push : plus de place dans la pile\n");
    	return 1;
    }
    programme->memoire[programme->SP]=programme->memoire[x];
    programme->SP += 1;
    return 0;
}


short ipush (Programme* programme){
    short sp = programme->SP-1;
    if (sp < 0){
        printf("ipush : erreur, il n'y a pas d'adresse en memoire\n");
        return 1;
    }
    short X = programme->memoire[sp];
    if (X < 0 || X > 4999){
        printf("ipush : Adresse non valide\n");
        return 1;
    }
    programme->memoire[sp] = programme->memoire[X];
    return 0;
}


short push_immediate (Programme* programme){
    if (programme->SP > 4999){
    	printf ("push_immediate : plus de place dans la pile\n");
    	return 1;
    }
    short i = programme->donnees[programme->PC-1];
    programme->memoire[programme->SP] = i;
    programme->SP += 1;
    return 0;
}


short jmp (Programme* programme){
    short adresse = programme->donnees[programme->PC-1];
    if (adresse == -1){
        printf ("jmp : boucle infini\n");
        return 1;
    }
    programme->PC += adresse;
    if (programme->PC < 0 || programme->PC > programme->nb_instructions-1){
        printf ("jmp : adresse invalide\n");
        return 1;
    }
    return 0;
}


short jnz (Programme* programme){
    programme->SP -= 1;
    if (programme->SP < 0){
        printf("jnz : erreur, il n'y a pas de valeur en memoire\n");
        return 1;
    }
    if (programme->memoire[programme->SP])
        return jmp (programme);
    return 0;
}

short call (Programme* programme){
    if (programme->SP > 4999){
    	printf ("call : plus de place dans la pile\n");
    	return 1;
    }
    programme->memoire[programme->SP] = programme->PC;
    programme->SP += 1;
    return jmp (programme);
}


short ret (Programme* programme){
    programme->SP -= 1;
    if (programme->SP < 0){
        printf("ret : erreur, il n'y a pas d'adresse en memoire\n");
        return 1;
    }
    programme->PC = programme->memoire[programme->SP];
    if (programme->PC < 0 || programme->PC > programme->nb_instructions-1){
        printf("ret : erreur, l'adresse n'est pas valide\n");
        return 1;
    }
    return 0;
}

short read (Programme* programme){
    short adresse = programme->donnees[programme->PC-1];
    if (adresse < 0 || adresse > 4999){
        printf("read : Adresse non valide\n");
        return 1;
    }
    printf("Entrez un entier entre -32768 et 32767 : ");
    scanf(" %hd", &(programme->memoire[adresse]));
    return 0;
}


short write (Programme* programme){
    short adresse = programme->donnees[programme->PC-1];
    if (adresse < 0 || adresse > 4999){
        printf("write : Adresse non valide\n");
        return 1;
    }
    printf("%hd\n", programme->memoire[adresse]);
    return 0;
}


short op (Programme* programme){
    short i = programme->donnees[programme->PC-1];
    if (i == 9 || i == 15){
        if (programme->SP-1 < 0){
            printf("op : erreur, il n'y a pas de valeur en memoire\n");
            return 1;
        }
        if (i == 9)
            non (programme);
        else 
            oppose (programme);
        return 0;
    }
    if (programme->SP-2 < 0){
        printf("op : erreur, il n'y a pas assez de valeurs en memoire\n");
        return 1;
    }
    if (i == 0){
        egal (programme);
        return 0;
    }
    if (i == 1){
        non_egal (programme);
        return 0;
    }
    if (i == 2){
        sup_egal (programme);
        return 0;
    }
    if (i == 3){
        inf_egal (programme);
        return 0;
    }
    if (i == 4){
        sup (programme);
        return 0;
    }
    if (i == 5){
        inf (programme);
        return 0;
    }
    if (i == 6){
        ou (programme);
        return 0;
    }
    if (i == 7){
        ou_exclusif (programme);
        return 0;
    }
    if (i == 8){
        et (programme);
        return 0;
    }
    if (i == 10){
        addition (programme);
        return 0;
    }
    if (i == 11){
        soustraction (programme);
        return 0;
    }
    if (i == 12){
        multiplication (programme);
        return 0;
    }
    if (i == 13){
    	if (programme->memoire[programme->SP-1] < 0){
    		printf ("op : division par 0 interdite\n");
    		
    		return 1;
    	}
        division (programme);
        return 0;
    }
    if (i == 14){
        modulo (programme);
        return 0;
    }
    printf ("op : code operation invalide\n");
    return 1;
}


short rnd (Programme* programme){
    short x = programme->donnees[programme->PC-1];
    if (x < 1){
    	printf("rnd : argument negatif interdit\n");
    	return 1;
    }
    if (programme->SP > 4999){
    	printf ("rnd : plus de place dans la pile\n");
    	return 1;
    }
    srand(time(NULL));
    programme->memoire[programme->SP] = rand () % x;
    programme->SP += 1;
    return 0;
}


short dup (Programme* programme){
    programme->memoire[programme->SP] = programme->memoire[programme->SP - 1];
    programme->SP += 1;
    return 0;
}
