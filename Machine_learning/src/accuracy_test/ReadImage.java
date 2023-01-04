package accuracy_test;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.DataOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.*;
import java.util.List;
import java.util.stream.IntStream;

public class ReadImage {
    static final int numberTesting = 55,numberLearning = 500;
    static final double T=0.4;
    static final double seed_value=6;
    String maskFolderName = "/home/sakib/Downloads/ibtd/Mask";
    String nonMaskFolderName = "/home/sakib/Downloads/ibtd";
    List<Integer> list=new ArrayList<Integer>();
    String maskImageList[],nonMaskImageList[];
    int[][][] screenColor,nonScreenColor;
    double[][][] result = new double[256][256][256];



    private void imageListgenerate()
    {
        maskImageList =  readImageFromFolder(maskFolderName);
        nonMaskImageList = readImageFromFolder(nonMaskFolderName);

        for(int i=0;i<=554;i++){
            list.add(i);
        }
    }

    private void readColorOfImage() throws IOException {
        Collections.shuffle(list, new Random(seed_value));

        screenColor = new int[256][256][256];
        nonScreenColor = new int[256][256][256];

        for(int i=0;i<numberLearning;i++) {
            String maskPathName = maskFolderName + "/" + maskImageList[list.get(i)];
            String nonMaskPathName = nonMaskFolderName + "/" + nonMaskImageList[list.get(i)];

            File maskImageFile = new File(maskPathName);
            File nonMaskImageFile = new File(nonMaskPathName);
            BufferedImage maskImage = ImageIO.read(maskImageFile);
            BufferedImage nonMaskImage = ImageIO.read(nonMaskImageFile);
            //System.out.println("Learning  "+list.get(i));

            for (int y = 0; y < maskImage.getHeight() && y< nonMaskImage.getHeight(); y++) {
                for (int x = 0; x < maskImage.getWidth() && x< nonMaskImage.getWidth(); x++) {
                    int maskPixel = maskImage.getRGB(x, y);
                    int nonMaskPixel = nonMaskImage.getRGB(x, y);

                    Color maskColor = new Color(maskPixel, true);
                    Color nonMaskColor = new Color(nonMaskPixel, true);

                    storeInArray(maskColor, nonMaskColor);
                }
            }
        }
    }


    private void storeInArray(Color maskColor, Color nonMaskColor)
    {
        int maskRed = maskColor.getRed();
        int maskGreen = maskColor.getGreen();
        int maskBlue = maskColor.getBlue();

        int nonMaskRed = nonMaskColor.getRed();
        int nonMaskGreen = nonMaskColor.getGreen();
        int nonMaskBlue = nonMaskColor.getBlue();

        if(maskRed>225 && maskGreen>225 && maskBlue>225){
            nonScreenColor[nonMaskRed][nonMaskGreen][nonMaskBlue]++;
        }
        else{
            screenColor[nonMaskRed][nonMaskGreen][nonMaskBlue]++;
        }

    }


    public void calculateProbability() throws IOException {
        int screenSum = getSum(screenColor);
        int nonScreenSum = getSum(nonScreenColor);
        double screenDouble,nonScreenDouble;

        result = new double[256][256][256];

        for(int x=0; x<=255; x++){
            for(int y=0; y<=255; y++){
                for(int z=0; z<=255;z++){
                    screenDouble = (double)screenColor[x][y][z]/screenSum;
                    nonScreenDouble = (double)nonScreenColor[x][y][z]/nonScreenSum;
                    if(nonScreenDouble != 0.0){
                        result[x][y][z] = screenDouble/nonScreenDouble;
                    }
                    else{
                        result[x][y][z] = 0.0;
                    }
                }
            }
        }

    }

    public int getSum(int colorArray[][][]){
        int total=0;
        for(int x=0; x<=255; x++){
            for(int y=0; y<=255; y++){
                total += Arrays.stream(colorArray[x][y]).sum();
            }
        }
        return total;
    }



    private String[] readImageFromFolder(String pathName)
    {
        File directoryPath = new File(pathName);
        String imageNames[] = directoryPath.list();
        Arrays.sort(imageNames);

        return imageNames;
    }

    int[][] confutionMatrix= new int[2][2];
    private double createConfutionMatrix() throws IOException {
        confutionMatrix[0][0] = 0;
        confutionMatrix[1][1] = 0;
        confutionMatrix[0][1] = 0;
        confutionMatrix[1][0] = 0;
        for(int i=numberLearning;i<=554;i++) {
            String maskPathName = maskFolderName + "/" + maskImageList[list.get(i)];
            String nonMaskPathName = nonMaskFolderName + "/" + nonMaskImageList[list.get(i)];

            File maskImageFile = new File(maskPathName);
            File nonMaskImageFile = new File(nonMaskPathName);
            BufferedImage maskImage = ImageIO.read(maskImageFile);
            BufferedImage nonMaskImage = ImageIO.read(nonMaskImageFile);
            //System.out.println(list.get(i));

            for (int y = 0; y < maskImage.getHeight() && y< nonMaskImage.getHeight(); y++) {
                for (int x = 0; x < maskImage.getWidth() && x< nonMaskImage.getWidth(); x++) {
                    int maskPixel = maskImage.getRGB(x, y);
                    int nonMaskPixel = nonMaskImage.getRGB(x, y);

                    switch (isScreenInMask(maskPixel)){
                        case 1:
                            switch (isScreenInTesting(nonMaskPixel)){
                                case 1:
                                    confutionMatrix[1][1]++;            //true positive
                                    break;
                                case 0:
                                    confutionMatrix[1][0]++;            //true negative
                                    break;
                            }
                            break;
                        case 0:
                            switch (isScreenInTesting(nonMaskPixel)){
                                case 1:
                                    confutionMatrix[0][1]++;            //false positive
                                    break;
                                case 0:
                                    confutionMatrix[0][0]++;            //false negative
                                    break;
                            }
                             break;
                    }
                }
            }

            //System.out.println("Testing---->> "+list.get(i));
        }

          int total = confutionMatrix[0][0]+confutionMatrix[0][1]+confutionMatrix[1][0]+confutionMatrix[1][1];
          System.out.println("\n\n\n"+confutionMatrix[0][0]+" "+confutionMatrix[1][0]+" "+confutionMatrix[0][1]+" "+confutionMatrix[1][1]+" Total "+total);
        double accuracy = ((double)(confutionMatrix[0][0]+confutionMatrix[1][1])/total);

        return accuracy;
    }


    private int isScreenInTesting(int pixel)
    {
        Color color = new Color(pixel, true);
        int red = color.getRed();
        int green = color.getGreen();
        int blue = color.getBlue();

        if(result[red][green][blue]> T){
            return 1;
        }
        else{
            return 0;
        }
    }

    private int isScreenInMask(int pixel)
    {
        Color color = new Color(pixel, true);
        int red = color.getRed();
        int green = color.getGreen();
        int blue = color.getBlue();

        if(red>225 && green>225 && blue>225){
            return 0;
        }
        else {
            return 1;
        }
    }

    public static void main(String[] args) throws IOException {
        long start,end;
        start=System.currentTimeMillis();

        double tim;
        double total_acuracy[] = new double[10], all_sum=0;
        for(int i=0;i<10;i++){
            ReadImage readImage = new ReadImage();
            readImage.imageListgenerate();
            readImage.readColorOfImage();
            readImage.calculateProbability();
            total_acuracy[i] = readImage.createConfutionMatrix();
            all_sum += total_acuracy[i];
            System.out.printf("Accuracy "+i+"th = "+total_acuracy[i]);
        }

        System.out.println("\n\n\n Total accuracy for 10th times = "+all_sum/10);

        end=System.currentTimeMillis();
        tim=(end-start)/1000.0;
        System.out.println("Total time = "+tim);

    }

}
