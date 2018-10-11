#include<bits/stdc++.h>
#include <mpi.h>
MPI_Status status;
using namespace std;

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
        int num;bool prime=true;
        cin>>num;

        //gettimeofday(&start,0);

        //send data to worker tasks
        int range=sqrt(num)/numworkers, l,r;

        for(dest=1;dest<=numworkers;dest++)
        {
            l=(dest-1)*range;r=(dest)*range;
            MPI_Send(&num,1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&l,1,MPI_INT,dest,1,MPI_COMM_WORLD);
            MPI_Send(&r,1,MPI_INT,dest,1,MPI_COMM_WORLD);
        }

        //wait for resultss from all worker tasks
        int isfactor;
        for(int i=1;i<=numworkers;i++)
        {
            source=i;
            MPI_Recv(&isfactor,1,MPI_INT,source,2,MPI_COMM_WORLD,&status);
            if(isfactor==1)prime=false;
        }

        //gettimeofday(&stop,0);
        cout<<num<<" is";
        if(!prime) cout<<" not";
        cout<<" prime\n";
    }

    //*******************WORKER*************************
    if(taskid>0)
    {
        source=0;
        int num,l,r,isfactor=0;
        MPI_Recv(&num,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&l,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);
        MPI_Recv(&r,1,MPI_INT,source,1,MPI_COMM_WORLD,&status);

        for(int k=l+1;k<=r;k++)
        {
            if(k!=1)
            {
                if(num%k==0)
                {
                    isfactor=1;
                    break;
                }
            }
        }

        MPI_Send(&isfactor,1,MPI_INT,0,2,MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return 0;
}
