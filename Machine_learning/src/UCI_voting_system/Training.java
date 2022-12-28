package UCI_voting_system;

import java.io.*;
import java.util.Objects;

public class Training {

    int[][][] confution_matrix = new int[16][2][2];
    int democrant=0,republican=0;
    double[][][] probability = new double[16][2][2];
    double democrant_probability,republic_probability;
    public void reader()
    {
        try
        {
            File file=new File("UCI Data/data.txt");    //creates a new file instance
            FileReader fr=new FileReader(file);   //reads the file
            BufferedReader br=new BufferedReader(fr);  //creates a buffering character input stream
            String line;
            while((line=br.readLine())!=null)
            {
                String[] str = line.split(",");
                create_confution_matrix(str);
            }
            fr.close();    //closes the stream and release the resources
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }
        calculate_probability();
    }


    public void create_confution_matrix(String[] str)
    {
        int index;
        if(Objects.equals(str[0], "democrat")) {
            democrant++;
            index = 0;
        }
        else {
            republican++;
            index = 1;
        }

        for(int i=0;i<=15;i++){
            if(Objects.equals(str[i + 1], "n"))
                confution_matrix[i][index][0]++;
            else
                confution_matrix[i][index][1]++;
        }
    }

    public void calculate_probability()
    {
//        System.out.println("demo: "+democrant+"  repub:  "+republican);
        democrant_probability = divition (democrant,democrant+republican);
        republic_probability = divition (republican,democrant+republican);
//        System.out.println(democrant_probability+" "+republic_probability);

        for(int i=0;i<16;i++){
            int temp = confution_matrix[i][0][0]+confution_matrix[i][1][0];
            probability[i][0][0] = divition (confution_matrix[i][0][0],temp);
            probability[i][1][0] = divition(confution_matrix[i][1][0],temp);


            temp = confution_matrix[i][0][1]+confution_matrix[i][1][1];
            probability[i][0][1] = divition(confution_matrix[i][0][1],temp);
            probability[i][1][1] = divition(confution_matrix[i][1][1],temp);
//            System.out.println(i+" th");
//            System.out.println(confution_matrix[i][0][0]+" "+confution_matrix[i][0][1]);
//            System.out.println(confution_matrix[i][1][0]+" "+confution_matrix[i][1][1]);
//
//            System.out.println(i+" th probability--->>>");
//            System.out.println(probability[i][0][0]+" "+probability[i][0][1]);
//            System.out.println(probability[i][1][0]+" "+probability[i][1][1]);
        }
    }

    public double divition(int a, int b)
    {
        double temp;
        if(b != 0)
            temp = (double)a/(double)b;
        else
            temp = 0.0;
        return temp;

    }

    public void estimate()
    {
        String str = "n,n,y,y,y,y,n,n,y,y,n,y,y,y,n,y";
        String[] str_split = str.split(",");
        double democrant_estimation = 1,republic_estimation=1;

        int i=0;
        for(String element: str_split){
            democrant_estimation *= probability[i][0][index(element)];
            republic_estimation *= probability[i++][1][index(element)];
        }
        democrant_estimation *= democrant_probability;
        republic_estimation *= republic_probability;
        System.out.println("Democrant: "+democrant_estimation+"   republi: "+republic_estimation);
        if(democrant_estimation>republic_estimation)
            System.out.println("Estimation is: democrant");
        else
            System.out.println("Estimation is: republican");
    }

    public int index(String st)
    {
        if(Objects.equals(st, "n"))
            return 0;
        else
            return 1;
    }






    public static void main(String[] args) throws IOException {
        Training training = new Training();
        training.reader();
        training.estimate();
    }
}
