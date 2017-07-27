package com.incarcloud;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

class DataRepo {
    private String _jdbc;
    private String _sql;

    DataRepo(){
        _jdbc = null;

        /* 最后所在位置
        SELECT A.obdCode, B.lastTime, substr(A.longitude,2) as lon, substr(A.latitude,2) as lat
        FROM t_obd_location A
        JOIN (SELECT obdCode, max(locationTime) lastTime
              FROM t_obd_location
              GROUP BY obdCode) B ON A.obdCode = B.obdCode and A.locationTime = B.lastTime
        */
        _sql = "SELECT A.obdCode, B.lastTime, substr(A.longitude,2) as lon, substr(A.latitude,2) as lat\n" +
                "FROM t_obd_location A\n" +
                "JOIN (SELECT obdCode, max(locationTime) lastTime\n" +
                "FROM t_obd_location\n" +
                "GROUP BY obdCode) B ON A.obdCode = B.obdCode and A.locationTime = B.lastTime";
    }

    void setDataSource(String jdbc, String user, String pwd)
    {
        _jdbc = String.format("%s&user=%s&password=%s", jdbc, user, pwd);
    }

    List<LastMark> fetchMarks() throws Exception{
        Connection conn = DriverManager.getConnection(this._jdbc);
        Statement statement = conn.createStatement();
        ResultSet rs = statement.executeQuery(this._sql);

        List<LastMark> listMarks = new ArrayList<>();
        while(rs.next()){
            try {
                String obdCode = rs.getString("obdCode");
                String tm = rs.getString("lastTime");
                double lon = rs.getDouble("lon");
                double lat = rs.getDouble("lat");

                LastMark mark = new LastMark(obdCode, tm, lon, lat);
                listMarks.add(mark);
            }
            catch(Exception ex){
                // ignore
            }
        }

        conn.close();

        return listMarks;
    }
}