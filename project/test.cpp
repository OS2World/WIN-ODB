#include <iostream.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	int i=0;
   FILE *fp;

   fp=fopen("test.txt","w");
   cout<<argc<<" Arguments "<<endl;
   while (i<argc)
   {
		cout<<argv[i]<<endl;
      fprintf(fp,"%s\n",argv[i]);
      i++;
   }

   fclose(fp);
   return 1;

}
