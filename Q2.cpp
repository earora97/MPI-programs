#include<bits/stdc++.h>
#include <mpi.h>
MPI_Status status;
using namespace std;
//#define INT_MAX 1000000000

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
        //input
        vector<int>a; string line;
        getline(cin,line);
        istringstream iss(line);int x;
        while(iss>>x) a.push_back(x);
        //send message to workers
        int n=a.size();
        int offset=0, range=n/numworkers,sent_size;
        for(dest=1;dest<=numworkers;dest++)
        {
            if(dest==numworkers)sent_size=n - numworkers*range + range; else sent_size=range;
            MPI_Send(&offset,1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&sent_size,1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&a[offset],sent_size,MPI_INT,dest,1,MPI_COMM_WORLD);
            offset+=range;
        }

        //wait for message from workers
        int b[n];int k=0; int c[n];
        for(int i=1;i<=numworkers;i++)
        {
            source=i;
            MPI_Recv(&offset,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);
            MPI_Recv(&sent_size,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);
            MPI_Recv(&b[offset],sent_size,MPI_INT,source,2,MPI_COMM_WORLD,&status);
            if(i!=1)
            {
                int l1=0,l2=offset,v1,v2;
                k=0;
                while(l1!=offset || l2!=offset+sent_size)
                {
                    if(l1!=offset)v1=b[l1]; else v1=100;
                    if(l2!=offset+sent_size)v2=b[l2]; else v2=100;
                    if(v1<v2)
                    {
                        c[k]=v1;l1++;k++;
                    }
                    else
                    {
                        c[k]=v2;l2++;k++;
                    }
                }
            }
            else
            {
                int l1=0;
                while(l1!=sent_size)
                {
                    c[k]=b[l1];l1++;k++;
                }
            }
            for(int x=0;x<offset+sent_size;x++)b[x]=c[x];
        }

        for(int i=0;i<n;i++)cout<<c[i]<<' ';
        cout<<'\n';
    }

    //*******************WORKER*************************
    if(taskid>0)
    {
        int offset,sent_size;
        source=0;
        MPI_Recv(&offset,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&sent_size,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        int a[sent_size];
        MPI_Recv(&a,sent_size,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        sort(a,a+sent_size);
        MPI_Send(&offset,1,MPI_INT,0,2,MPI_COMM_WORLD);
        MPI_Send(&sent_size,1,MPI_INT,0,2,MPI_COMM_WORLD);
        MPI_Send(&a,sent_size,MPI_INT,0,2,MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
