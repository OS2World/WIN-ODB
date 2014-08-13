

int chartype::validate(char *st, void *retv){
	retv=(void *) st;
	return 1;
}

int realtype::validate(char *inp, void *retv){
int length=strlen(inp);
int dotpos, currpos=0,saveddotpos;
float retval=0.0;
float ffactor;
char currch=inp[currpos];
	dotpos=-1;
	while((currpos<length)&&(currch!='\0')){  //where is decimal point
		if (currch=='.') dotpos=currpos;
		currpos++;
		currch=inp[currpos];
		}
	currpos=dotpos+1;
	ffactor=1.0/10.0;
	currch=inp[currpos];
	while((currpos<length)&&(currch!='\0')){
		if (isdigit(currch)) {
			retval=retval+(currch-48)*ffactor;
			ffactor=ffactor/10.0;
			currpos++;
			currch=inp[currpos];
			}
		else currpos=2*length; //signals shitty input
	}; //end while
	if (currpos==2*length) {
		return 0;
		}
	else {  //get intpart add add it to realpart
		inp[dotpos]='\0';
		saveddotpos=dotpos;
		dotpos=0;
		if (validate_integer(inp,dotpos)!=0){ //reuse dotpos;
			retval=retval+dotpos;
			*retv=retval;
			inp[saveddotpos]='.';
			return 1;
			}
		else {
			inp[saveddotpos]='.';
			return 0;
			}
	}
}


int inttype::validate(char *st, void *retv){
int length=strlen(inp);
int currpos=0;
int retval=0;
int factor=pow10(length-1);
char currch=inp[currpos];
	while((currpos<length)&&(currch!='\0')){
		if (isdigit(currch)) {
			retval=retval+(currch-48)*factor;
			factor=factor/10;
			currpos++;
			currch=inp[currpos];
			}
		else currpos=2*length; //signals shitty input
	}; //end while
	if (currpos==2*length) {
		return 0;
		};
	*value=retval;
	return 1;
}

