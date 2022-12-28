package imageProcessing;

import javax.imageio.ImageIO;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.*;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class Testing {

    double[][][] resultValue;
    int b=0,w=0;
    static final double T = 0.4;

    public void loadData() throws IOException {
        FileInputStream inputStream = null;
        resultValue = new double[256][256][256];

        inputStream = new FileInputStream("Probability.txt");
        DataInputStream dataInputStr = new DataInputStream(inputStream);

        for(int x=0; x<=255; x++){
            for(int y=0; y<=255; y++){
                for(int z=0; z<=255;z++){
                    resultValue[x][y][z] = dataInputStr.readDouble();
                }
            }
        }
    }


    public void makeImage(String path) throws IOException {
        File imageFile = new File(path);
        BufferedImage img = ImageIO.read(imageFile);


        for (int y = 0; y < img.getHeight() ; y++) {
            for (int x = 0; x < img.getHeight() ; x++) {
                int pixel = img.getRGB(x, y);

                int rgb = getReplaceColor(pixel);
                img.setRGB(x,y,rgb);

            }
        }
    }


    public void newImageMake(String path) throws IOException {

        File imageFile = new File(path);
        BufferedImage img = ImageIO.read(imageFile);

        int width = img.getWidth();
        int height = img.getHeight();

        BufferedImage newImage = new BufferedImage(width, height, img.getType());
        int color;

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                color = img.getRGB(i, j);
                color = getReplaceColor(color);

                newImage.setRGB(i, j, color);
            }
        }
        ImageIO.write(newImage, "jpg", new File("48.jpg"));
    }


    public int getReplaceColor(int pixel)
    {
        Color color = new Color(pixel, true);
        int red = color.getRed();
        int green = color.getGreen();
        int blue = color.getBlue();

        if(resultValue[red][green][blue] < T) {
            pixel = new Color(255, 255, 255).getRGB();
        }

        return pixel;
    }



    public static void main(String[] args) throws IOException {
        Testing testing = new Testing();
        testing.loadData();
        testing.newImageMake("0048.jpg");

        //System.out.println("Total black "+testing.b+"  white: "+testing.w);
    }
}
