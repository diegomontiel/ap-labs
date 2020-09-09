#include <stdio.h>

int main(){
    // Place your magic here
    char c;
    char word[100]="";
    int count = 0;
    while((c = getchar()) != EOF){
	    if(c == '\n'){
		    for(int i = count; i >= 0; i--){
			    printf("%c", words[i]);
		    }
		    printf("/n");
		    memset(words, 0, sizeof words);
	    } else {
		    words[count] = c;
		    count++;
	    }
    }
    return 0;
}
