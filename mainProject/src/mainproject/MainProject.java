/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package mainproject;
import java.io.*;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import java.util.Date;


/**
 *
 * @author Evan
 */
public class MainProject {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        
        Scanner input = new Scanner(System.in);
        Date date = new Date();
        
        int PoW, numOfCycles = 0;
        
        try{
                        
            System.out.println("\n\nEvan Fahy.");
            TimeUnit.SECONDS.sleep(1);
            System.out.println("G00287967.\n");
            TimeUnit.SECONDS.sleep(1);
            System.out.println(date.toString());
            TimeUnit.SECONDS.sleep(1);
            System.out.println("Final year project.");
            TimeUnit.SECONDS.sleep(1);
            System.out.println("Precision control circuit for a high current, short circuit tester.");
            TimeUnit.SECONDS.sleep(1);
            
        }catch(Exception e){
        
            e.printStackTrace();
        }
        try {
            
            File file = new File("dist/testData.txt");
            
            FileWriter writer = new FileWriter(file, false);
            PrintWriter out = new PrintWriter(writer);
            
            out.write("/" + date.toString());
            
            System.out.println("\nPlease enter desired Point-On-Wave(PoW) in degrees.");
            System.out.println("Point-On-Wave(PoW) must be between 10°-350°:");
            boolean validPoW = false;
            
            
            while(validPoW == false)
            {
                PoW = input.nextInt();
                if ((PoW < 10) || (PoW > 350)){
                
                    System.out.println("Invalid entry. 10°-350°.");
                    validPoW = false;
                }
                else{
                    validPoW = true;
                    System.out.println("User entered " + PoW + "°.");
                    out.write("/PoW:" + Integer.toString(PoW));
                }
            }
            
            System.out.println("\nPlease enter desired number of half cycles for test.");
            System.out.println("Cycles must be between 1-10:");
            boolean validCycleCount = false;
            
            
            while(validCycleCount == false)
            {
                numOfCycles = input.nextInt();
                if ((numOfCycles < 1) || (numOfCycles > 10)){
                
                    System.out.println("Invalid entry. 1-10.");
                    validCycleCount = false;
                }
                else{
                    validCycleCount = true;
                    System.out.println("User entered " + numOfCycles + " cycles.");
                    out.write("/numOfCycles:" + Integer.toString(numOfCycles) + "\n");
                }
            }
            
            System.out.println("-----------------");

            TimeUnit.SECONDS.sleep(1);            

            out.close();
        } 
        catch (Exception e) {
            
            e.printStackTrace();
        }
        
        //Upload to Database
        
    }   
}