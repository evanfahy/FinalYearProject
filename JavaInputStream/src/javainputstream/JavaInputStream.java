/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package javainputstream;

/**
 *
 * @author Evan
 */

import java.io.*;
import java.util.Date;
import java.util.Scanner;
import java.util.concurrent.TimeUnit;
import java.sql.*;

public class JavaInputStream {

    /**
     * @param args the command line arguments
     */
 public static void main(String[] args) throws IOException {
        
     Scanner input = new Scanner(System.in);
       
        String PoW = ""; 
        String cycles = "";
        String comments = "";
        String textIn;
        
        try {
            
            File file = new File("dist/testData.txt");
            
            FileReader reader = new FileReader(file);
            BufferedReader in = new BufferedReader(reader);
            
            textIn = in.readLine();
            
            String delims = "[/]";
            String[] t0 = textIn.split(delims);
           
            delims = "[:]";
            String[] t1 = t0[2].split(delims);
            String[] t2 = t0[3].split(delims);
          
            PoW = t1[1];
            cycles = t2[1];
            
            System.out.println("Uploading test parameters to database @pi:3306/testResults");
            System.out.println("Enter additional comments? (Y/N)");
            boolean upload = false;
            String choice = "";
            
            while(upload == false){
            choice = input.nextLine();
            if(choice.equals("Y") || choice.equals("y")){
            
                System.out.print("Comments:");
                comments = input.nextLine();
                upload = true;
            }
            else if(choice.equals("N")|| choice.equals("n")){
            
                upload = true;
            }
            else
                System.out.println("Invalid option!");
            }
           
            in.close();
        } 
        catch (Exception e) {
            
            e.printStackTrace();
        }
        try{
        //Upload to Database
        String host = "jdbc:mysql://localhost:3306/testResults";
    String uname = "phpmyadmin";
    String pword = "root";
    
    Connection connection;
    
    Class.forName( "com.mysql.jdbc.Driver" );
    connection = DriverManager.getConnection(host, uname, pword);
    
    PreparedStatement prepareStatement;
    prepareStatement = connection.prepareStatement("INSERT INTO results (point_on_wave, cycles, comments) VALUES ( '"+PoW+"' , '"+cycles+"' , '"+comments+"' ) ");
    prepareStatement.executeUpdate();
        }catch(Exception ee){
        
            ee.printStackTrace();
        }
        
    }   
}
