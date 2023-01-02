package imageProcessing;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.Arrays;

public class ImageProcess {

        int[][][] screenColor,nonScreenColor;
        double[][][] screenResult, nonScreenResult;

        private void readImage() throws IOException {
            String maskFolderName = "ibtd/Mask";
            String nonMaskFolderName = "ibtd";

            screenColor = new int[256][256][256];
            nonScreenColor = new int[256][256][256];



            //String maskImageList[], nonMaskImageList[];
           String maskImageList[] =  readImageFromFolder(maskFolderName);
           String nonMaskImageList[] = readImageFromFolder(nonMaskFolderName);

           for(int i=0;i<=554;i++) {

               String maskPathName = maskFolderName + "/" + maskImageList[i];
               String nonMaskPathName = nonMaskFolderName + "/" + nonMaskImageList[i];
               //System.out.println(maskPathName+"\n"+nonMaskPathName);

               File maskImageFile = new File(maskPathName);
               File nonMaskImageFile = new File(nonMaskPathName);
               BufferedImage maskImage = ImageIO.read(maskImageFile);
               BufferedImage nonMaskImage = ImageIO.read(nonMaskImageFile);
               System.out.println(i);
//            System.out.println(maskImage.getHeight()+"  "+nonMaskImage.getHeight());
//            System.out.println(maskImage.getWidth()+"   "+nonMaskImage.getWidth());
               for (int y = 0; y < maskImage.getHeight() && y< nonMaskImage.getHeight(); y++) {
                   for (int x = 0; x < maskImage.getHeight() && x< nonMaskImage.getWidth(); x++) {
                       int maskPixel = maskImage.getRGB(x, y);
                       int nonMaskPixel = nonMaskImage.getRGB(x, y);

                       Color maskColor = new Color(maskPixel, true);
                       Color nonMaskColor = new Color(nonMaskPixel, true);

                       storeInArray(maskColor, nonMaskColor);
                   }
               }
           }

        }


        public void storeInArray(Color maskColor, Color nonMaskColor)
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
                //System.out.println(nonMaskRed+" "+nonMaskGreen+" "+nonMaskBlue);
            }

            //screenColor[0][0][0] = 5;

        }

        public void calculateProbability() throws IOException {
            int screenSum = getSum(screenColor);
            int nonScreenSum = getSum(nonScreenColor);
            double tempResult;

             screenResult = new double[256][256][256];
             nonScreenResult  = new double[256][256][256];


            divide(screenSum,screenResult,screenColor);
            divide(nonScreenSum,nonScreenResult,nonScreenColor);

            FileOutputStream outputStream = new FileOutputStream("Probability.txt");
            DataOutputStream dataOutputStr = new DataOutputStream(outputStream);

            for(int x=0; x<=255; x++){
                for(int y=0; y<=255; y++){
                    for(int z=0; z<=255;z++){
                        if(nonScreenResult[x][y][z] != 0.0)
                            tempResult = screenResult[x][y][z] / nonScreenResult[x][y][z];
                        else
                            tempResult =0.0;

                        dataOutputStr.writeDouble(tempResult);
                        //System.out.print(tempResult);
                    }
                }
                //System.out.println(x+"-->>");
            }

            dataOutputStr.flush();

        }

        public void divide(int total, double resultArray[][][], int counter[][][])
        {
            for(int x=0; x<=255; x++){
                for(int y=0; y<=255; y++){
                    for(int z=0; z<=255;z++){
                        resultArray[x][y][z] =(double) counter[x][y][z]/ total;
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

    public static void main(String[] args) throws IOException {
        ImageProcess imageProcess = new ImageProcess();
        imageProcess.readImage();
        imageProcess.calculateProbability();
    }
}
