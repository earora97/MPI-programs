#include<bits/stdc++.h>
#include <mpi.h>
MPI_Status status;
using namespace std;

double calc_area(int x1,int y1,int x2,int y2,int x3, int y3)
{
    double a=sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)),b=sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3)),c=sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
    double s=(a+b+c)/2;
    double area = sqrt(s*(s-a)*(s-b)*(s-c));
    return area;
}

int main(int argc, char ** argv)
{
    int numtasks, taskid, numworkers, source, dest;
    //struct timeval start,stop;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    numworkers=numtasks-1;

    //***********************MASTER***************************
    if(taskid==0)
    {
        int n,a,b; vector<int>x;vector<int>y;
        cin>>n;
        for(int i=0;i<n;i++)
        {
            cin>>a>>b;
            x.push_back(a);y.push_back(b);
        }
        int range=(n-1)/numworkers,sent_size,offset=1;
        double area=0,arean;

        //send message to worers
        for(dest=1;dest<=numworkers;dest++)
        {
            if(dest==numworkers)sent_size= (n-1)-range*numworkers +1 + range; else sent_size=range+1;
            MPI_Send(&offset,1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&sent_size,1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&x[0],1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&y[0],1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&x[offset-1],sent_size,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&y[offset-1],sent_size,MPI_INT,dest,1,MPI_COMM_WORLD);
            offset+=range;
        }

        //recieve message from workers
        for(int i=1;i<=numworkers;i++)
        {
            source=i;
            MPI_Recv(&arean,1,MPI_DOUBLE,source,2,MPI_COMM_WORLD,&status);
            area+=arean;
        }

        cout<<"Area: "<<area<<endl;
    }

    //*******************WORKER*************************
    if(taskid>0)
    {
        int offset,sent_size,a,b;
        source=0;
        MPI_Recv(&offset,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&sent_size,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&a,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&b,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        int x[sent_size];int y[sent_size];
        MPI_Recv(&x,sent_size,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&y,sent_size,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        int l; if(offset==1)l=1; else l=0;
        double area=0;
        for(;l+1<sent_size;l++)area+=calc_area(a,b,x[l],y[l],x[l+1],y[l+1]);
        MPI_Send(&area,1,MPI_DOUBLE,0,2,MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
