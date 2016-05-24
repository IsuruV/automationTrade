#ifndef MA_H
#define MA_H

int x[20]; 
int y[100];

int xx, yy, xAv,yAv;

List<double>^ 100MA;
List<double>^ 20MA;
100MA= gcnew List<double>();
20MA = gcnew List<double>();
for (int count =0;count<20;count++)
{
	xx += x[count]; 
}
xAv = xx/20;
20MA = XAv;
for (int count =0; count<100; count++)
{
	yy +=y[count];
}
yAv = yy/100; 
100MA = yAv;

while ( 20MA > 100MA)
{
	if (xAv == yAv)
	{

	//sell; death cross
	}
}

while (100MA > 20MA)
{
	if (XAv == yAv)
	{
		// buy;  golden cross
	}
}



#endif /* MA_H */