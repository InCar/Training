package com.incarcloud;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.*;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.sql.*;

@SpringBootApplication
public class App implements CommandLineRunner {
    public static void main(String[] args) {
        SpringApplication.run(App.class, args);
    }

    @Autowired
    private DatabaseConfig _cfg;

    @Autowired
    private WuhanConfig _wuhan;

    public void run(String... args) throws Exception{
        System.out.println("=====begin=====");
        Connection conn = DriverManager.getConnection(_cfg.DataBase
                + "&user=" + _cfg.User
                + "&password=" + _cfg.Password);

        Statement statement = conn.createStatement();
        /* 最后所在位置
        SELECT A.obdCode, B.lastTime, substr(A.longitude,2) as lon, substr(A.latitude,2) as lat
        FROM t_obd_location A
        JOIN (SELECT obdCode, max(locationTime) lastTime
              FROM t_obd_location
              GROUP BY obdCode) B ON A.obdCode = B.obdCode and A.locationTime = B.lastTime
        */
        String sql = "SELECT A.obdCode, B.lastTime, substr(A.longitude,2) as lon, substr(A.latitude,2) as lat\n" +
                     "FROM t_obd_location A\n" +
                     "JOIN (SELECT obdCode, max(locationTime) lastTime\n" +
                           "FROM t_obd_location\n" +
                           "GROUP BY obdCode) B ON A.obdCode = B.obdCode and A.locationTime = B.lastTime";
        ResultSet rs = statement.executeQuery(sql);
        while(rs.next()){

            try {
                double dLon = rs.getDouble("lon");
                double dLat = rs.getDouble("lat");

                if (dLon >= _wuhan.left && dLon <= _wuhan.right
                        && dLat >= _wuhan.bottom && dLat <= _wuhan.top) {
                    String strCode = rs.getString("obdCode");
                    String strTm = rs.getString("lastTime");

                    System.out.println(strCode + " is in Wuhan at " + strTm);
                }
            }
            catch(Exception ex){
                // ignore
            }

        }
        System.out.println("=====end=====");
    }
}
