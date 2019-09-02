#include<stdio.h>
#include<stdlib.h>

struct link     //this structure will keep data in it
{
    int data;
    struct link *next;
};

struct node1        //this structure will keep feature name and link and self pointing pointers
{
    int feature_name;
    struct link *link;
    struct node1 *next;
    
};
struct keep_prob        //this structure will keep feature name and number of yes and no's and self pointing pointers
{
    int feature;
    float feature_yes;
    float feature_no;
    struct keep_prob *next;
};


struct cell     //this struture will keep feature and self pointing pointer
{
    int feature;
    struct node1 *ptr;
};
struct cell2         //this struture will keep feature and self pointing pointer
{
    int feature;
    struct keep_prob *ptr;
};
struct cell values[5]; //array of structure of type struct cell
struct cell2 prob[4];   //array of structure of type struct cell2

struct node1* make_posting_list(FILE *fp,int no_of_features)
{
   //this function is used to store the data from the file and storing it in form of posting list
    int array[14];  //a temperory array to store the values of the file
    int i=0;
    char ch;
    //fgetc from file to store data in the array
    while((ch = fgetc(fp)) != EOF)
    {
        array[i] = (int)(ch-'0');
        i++;
    }
   
   struct node1 *parent =NULL;
   struct node1 *head;
    //creating empty posting lists to store the data
    for(int i=1;i<=no_of_features;i++)
    {
        
        struct node1 *node1_temp1 = (struct node1 *)malloc(sizeof(struct node1));
        node1_temp1->feature_name = i;
        node1_temp1->next =NULL;
        node1_temp1->link = NULL;
     
        
        if(parent==NULL)
        {
            head = node1_temp1;
            parent = head;
        }
        else
        {
            while(head->next !=NULL)
            {
                head = head->next;
            }
            head->next = node1_temp1;
            head=parent;
                
        }
    }

    int x=0;
    struct link *head2 = NULL; 
    struct link *parent2 = NULL; 
    struct link *head3 = NULL; 
    struct link *parent3 = NULL;
    struct link *head4 = NULL; 
    struct link *parent4 = NULL;
    int seq_no =1;  //use of this variable is to denote the place of feature in the file
    //inserting the data in the postings
    while(x<14)
    {
        struct link *temp2 = (struct link *)malloc(sizeof(struct link));
        temp2->next = NULL;
        temp2->data = 0;
        
        if(array[x]==1)
        {
            if(parent2 == NULL)
            {
            temp2->data = seq_no;
            seq_no++;
            head2 = temp2;
            parent2 = temp2;
            }
            else
            {
            while(head2->next !=NULL)
            {
                head2 = head2->next;
            }
            head2->next = temp2;
            temp2->data = seq_no;
            seq_no++;
            head2 = parent2;    
            }
        }
        else if(array[x]==2)
        {
            if(parent3 == NULL)
            {
            temp2->data = seq_no;
            seq_no++;
            head3 = temp2;
            parent3 = temp2;
            }
            else
            {
            while(head3->next !=NULL)
            {
                head3 = head3->next;
            }
            head3->next = temp2;
            temp2->data = seq_no;
            seq_no++;
            head3 = parent3;    
            }
        }
       
        else if(array[x]==3)
        {
            if(parent4 == NULL)
            {
            temp2->data = seq_no;
            seq_no++;
            head4 = temp2;
            parent4 = temp2;
            }
            else
            {
            while(head4->next !=NULL)
            {
                head4 = head4->next;
            }
            head4->next = temp2;
            temp2->data = seq_no;
            seq_no++;
            head4 = parent4;    
            }
        }
        
    x++;
            
    }
    
    struct node1 *papa = parent;
    //linking different sections together
    while (parent != NULL)
    {
        if(parent->feature_name == 1)
        {
        parent->link = parent2;
        parent=parent->next;
    
        }
        else if(parent->feature_name ==2 )
        {
        parent->link = parent3;
        parent=parent->next;
    
        }
        else if(parent->feature_name ==3 )
        {
        parent->link = parent4;
        parent=parent->next;
    
        }

        
    }
    
    return papa;
}
int calculate(struct link *p,struct link *q)
{
    //this function is just counting the similar cases between 2 posting list 
    int count =0;
    
    while(p != NULL && q!= NULL)
    {
        if(p->data == q->data)
        {
    
            count++;
    
            p=p->next;
            q=q->next;
        }
        else if((p->data)>(q->data))
        {
    
            q=q->next;
        }
        else
        {
    
            p=p->next;
        }
    }
    
    return count;   //returns the count
}
struct keep_prob * merge(struct node1 *start,struct node1 *end,int yes,int no,int no_of_features)
{
    //this function makes a posting list for the probabilities and calculate the probabilities
    struct node1 *travel1 = start; //copying the pointer so that we don't lose them! it's a good practice
    struct node1 *travel2 = end;

    struct keep_prob *parent = NULL; // initializing the pointer to NULL
    struct keep_prob *head = NULL;  // initializing the pointer to NULL
    // this loop creates the posting list for the probabilities
    for(int i=1;i<=no_of_features;i++)
    {
        
        struct keep_prob *temp1 = (struct keep_prob *)malloc(sizeof(struct keep_prob));
        temp1->feature = i;
        temp1->feature_no = 0;
        temp1->feature_yes=0;
        temp1->next =NULL;
        
        
        if(parent==NULL)
        {
            head = temp1;
            parent = head;
        }
        else
        {
            while(head->next !=NULL)
            {
                head = head->next;
            }
            head->next = temp1;
            head=parent;
                
        }
    }
    

    struct keep_prob *r = parent;   //storing the starting pointer to a posting list
    //this loop calculates the probabilities for the keep_prob
    while (travel1 != NULL)
    {
        struct link *p = travel1->link;
        struct link *q = travel2->link;
        float num1,num2;
    
        num1 =  calculate(p,q);
           r->feature_yes = num1/yes;
            p = travel1->link;
           travel2= travel2->next;
            q = travel2->link;
    
            num2 = calculate(p,q);
            r->feature_no = num2/no;
            r = r->next;
            travel1 = travel1->next;
            travel2 = end;
    }
    
    return parent;  //returning the parent node
}

int main()
{
    for(int i=0;i<5;i++)
    {
        values[i].feature = i;      //initializing the array of structure cell with features
    }

    for(int i=0;i<5;i++)
    {
        prob[i].feature = i;        //initializing the array of structure keep_prob with features
    }
    FILE *fp1,*fp2,*fp3,*fp4,*fp5;      //file pointers to the files
    int seq_no=1;       // denotes the location of the feature in the file
    fp1 = fopen("outlook.txt","r"); // opening the outlook.txt file
    fp2 = fopen("temperature.txt","r"); // opening the temperature.txt file
    fp3 = fopen("humidity.txt","r");    // opening the humidity.txt file
    fp4 = fopen("wind.txt","r");    // opening the wind.txt file
    fp5 = fopen("output.txt","r");  // opening the output.txt file
    // this loop creates the posting list for the respective files
    for(int i=0;i<5;i++)
    {
        if(i==0)
        {
        values[i].ptr = make_posting_list(fp1,3); // making the posting list of the respective file
        }
        else if(i==1)
        {
            values[i].ptr = make_posting_list(fp2,3);   // making the posting list of the respective file
        }
        else if(i==2)
        {
            values[i].ptr =make_posting_list(fp3,2);    // making the posting list of the respective file
         
        }
        else if(i==3)
        {
            values[i].ptr = make_posting_list(fp4,2);   // making the posting list of the respective file
    
        }
        else if(i==4)
        {
            values[i].ptr =make_posting_list(fp5,2);    // making the posting list of the respective file
        }

    }

    

    int total_yes = 9, total_no =5; //variables denoting the total number of yes and no's
    //this loop compares the two lists to calculate the conditional probabilities
    for(int i=0;i<4;i++)
    {
        if(i==0)
        {

        prob[i].ptr = merge(values[i].ptr,values[4].ptr,total_yes,total_no,3);  //calculating P(<sunny,overcast,rain>/yes)
        
        }
        else if(i==1)
        {
        prob[i].ptr = merge(values[i].ptr,values[4].ptr,total_yes,total_no,3);  //calculating conditional probability
        
        }
        else if(i==2)
        {
        prob[i].ptr = merge(values[i].ptr,values[4].ptr,total_yes,total_no,2);  //calculating conditional probability
        }
        else if(i==3)
        {
        
        prob[i].ptr = merge(values[i].ptr,values[4].ptr,total_yes,total_no,2);  //calculating conditional probability
        }
        

    }
    
    double prob_yes = 9.0/14.0;     //probability of yes
    double prob_no = 5.0/14.0;      //probability of no
    
    //rules for entering the data
    printf("enter the features in order outlook,temperature,humidity and wind\n");
    printf("please read the instruction before entering the values\n");
      printf("************************************************\n");
        printf("outlook = 0 | sunny = 1|overcast = 2 |rain = 3\n");
       printf(" temperature = 1 | hot = 1|mild = 2 |cool = 3\n");
       printf("humidity = 2 | high =1 | normal = 2\n");
       printf("wind = 3 | srtong =1 | weak = 2\n");
       printf("************************************************\n");

       int feature[4]; //features entered by the user
       scanf("%d%d%d%d",&feature[0],&feature[1],&feature[2],&feature[3]);
    //calculating the result as per the given input
       double yes=1,no=1; 
       for(int i=0;i<4;i++)
       {
           struct keep_prob *h=prob[i].ptr;
           while(h->feature != feature[i])
           {
               h=h->next;
           }
           yes = yes*(h->feature_yes);
           no = no*(h->feature_no);
       }    
       yes = yes*prob_yes;
       printf("prob of yes = %lf\n",yes);
       
       no = no*prob_no;
       printf("prob of no = %lf\n",no);
       if(yes>no)
       printf("You can play tennis!\n");
       else
       {
           printf("you can not play tennis!\n");
       }
       
        




    return 0;

}
