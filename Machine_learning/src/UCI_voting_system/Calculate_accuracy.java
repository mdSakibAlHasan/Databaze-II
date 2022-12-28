package UCI_voting_system;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

public class Calculate_accuracy {
    final static int training_data=392,testing_data=43;
    List<Integer> list=new ArrayList<Integer>();
    String[][] line = new String[450][];
    int[][][] confution_matrix = new int[16][2][2];
    int democrant=0,republican=0;
    double[][][] probability = new double[16][2][2];
    double democrant_probability,republic_probability;
    Training training = new Training();
    int success=0;
    public void read_all_line()
    {
        try
        {
            File file=new File("UCI Data/data.txt");    //creates a new file instance
            FileReader fr=new FileReader(file);   //reads the file
            BufferedReader br=new BufferedReader(fr);  //creates a buffering character input stream
            String str;
            int i=0;
            while((str=br.readLine())!=null)
            {
                line[i++] = str.split(",");
            }
            fr.close();    //closes the stream and release the resources
        }
        catch(IOException e)
        {
            e.printStackTrace();
        }

    }

    public void randomize()
    {
        for(int i=0;i<435;i++){
            list.add(i);
        }
        Collections.shuffle(list);
    }

    public int estimate(String str_split[])
    {
//        String str = "n,n,y,y,y,y,n,n,y,y,n,y,y,y,n,y";
//        String[] str_split = str.split(",");
        double democrant_estimation = 1,republic_estimation=1;

        for(int i=0;i<16;i++){
            democrant_estimation *= probability[i][0][training.index(str_split[i+1])];
            republic_estimation *= probability[i][1][training.index(str_split[i+1])];
        }
        democrant_estimation *= democrant_probability;
        republic_estimation *= republic_probability;
        //System.out.println("Democrant: "+democrant_estimation+"   republi: "+republic_estimation);
        if(democrant_estimation>republic_estimation)
            return 0;
        else
            return 1;
    }


    public void calculate_probability()
    {
//        System.out.println("demo: "+democrant+"  repub:  "+republican);
        democrant_probability = training.divition (democrant,democrant+republican);
        republic_probability = training.divition (republican,democrant+republican);
//        System.out.println(democrant_probability+" "+republic_probability);

        for(int i=0;i<16;i++){
            int temp = confution_matrix[i][0][0]+confution_matrix[i][1][0];
            probability[i][0][0] = training.divition (confution_matrix[i][0][0],temp);
            probability[i][1][0] = training.divition(confution_matrix[i][1][0],temp);


            temp = confution_matrix[i][0][1]+confution_matrix[i][1][1];
            probability[i][0][1] = training.divition(confution_matrix[i][0][1],temp);
            probability[i][1][1] = training.divition(confution_matrix[i][1][1],temp);
        }
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
            if(Objects.equals(str[i+1], "n"))
                confution_matrix[i][index][0]++;
            else
                confution_matrix[i][index][1]++;
        }
    }

    public void compare(int result, int index)
    {
        if(Objects.equals(line[list.get(index)],"democrat")){
            if(result == 0)
                success++;
        }
        else {
            if(result == 1)
                success++;
        }
    }

    public void controller()
    {
        double total=0;
        for(int k=0;k<10;k++) {
            randomize();
            for (int i = 0; i < training_data; i++) {
                //System.out.println(Arrays.toString(line[list.get(i)])+" "+list.get(i));
                create_confution_matrix(line[list.get(i)]);
            }
            calculate_probability();
            for (int i = training_data; i < training_data + testing_data; i++) {
                compare(estimate(line[list.get(i)]), i);
            }
            double temp = (double) success / (double) testing_data;
            success = 0;
            System.out.println("Accuracy of "+k+" :"+temp);
            total +=temp;
        }

        System.out.println("\n\n Total success rate: "+total/10);


    }

    public static void main(String[] args) {
        Calculate_accuracy calculateAccuracy = new Calculate_accuracy();
        calculateAccuracy.read_all_line();
        calculateAccuracy.controller();
    }
}
