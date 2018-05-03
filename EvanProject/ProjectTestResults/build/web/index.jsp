<%@page import="java.sql.*"%>
<%@page import="java.sql.Connection"%>
<%@page import="java.sql.DriverManager"%>

<%
    String host = "jdbc:mysql://localhost:3306/testResults";
    String uname = "phpmyadmin";
    String pword = "root";
    
    Connection connection;
    
    Class.forName( "com.mysql.jdbc.Driver" );
    connection = DriverManager.getConnection(host, uname, pword);
    
    PreparedStatement selectStatement;

%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <title>Test Results</title>
        <link rel="stylesheet" href="styling.css">
    </head>

    <body>
        <div class="box">
            <h1>Test Results</h1>
            <div style="margin: 0px 20px 10px;">
            
            <%
                try{
                    selectStatement = connection.prepareStatement("SELECT ID, DATE_FORMAT(time_of_test, '%e %M %Y') time_of_test, point_on_wave, cycles, comments FROM results ");
                    ResultSet resultSet = selectStatement.executeQuery();
                    
                    if (resultSet.isBeforeFirst()) {    

                        out.print("<table><tr><th>Time Of Test</th><th>Point On Wave</th><th>Cycles</th><th>Comments</th></tr>");

                        while(resultSet.next()){
                            out.print("<tr>"
                                    + "<td>" + resultSet.getString("time_of_test") + "</td>"
                                    + "<td>" + resultSet.getString("point_on_wave") + "</td>"
                                    + "<td>" + resultSet.getString("cycles") + "</td>"
                                    + "<td style='width:55%'>" + resultSet.getString("comments") + "</td>"
                                    + "</tr>");   
                        }

                        out.print("</table>");
                    } else {
                        
                        out.print("Table is empty");
                    }

                    resultSet.close();
                
                }catch(Exception e){
                   e.printStackTrace();
                }
            %>  
       
            </div>

        </div>	
    </body>
</html>
