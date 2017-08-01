package com.incarcloud.my.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */

import com.incarcloud.core.IDataRepo;
import com.incarcloud.core.geo.IGeoMark;

import java.sql.*;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Map;

/**
 * @author Fan Beibei
 * @Description: 描述
 * @date 2017/7/31 16:20
 */
public class MyGeoDataRepo implements IDataRepo<IGeoMark> {


    private String jdbc;
    private String sql;

    public MyGeoDataRepo() {

        sql = "SELECT A.obdCode, B.lastTime, substr(A.longitude,2) as lon, substr(A.latitude,2) as lat\n" +
                "FROM t_obd_location A\n" +
                "JOIN (SELECT obdCode, max(locationTime) lastTime\n" +
                "FROM t_obd_location t0 \n" +
                "GROUP BY obdCode" +
                ") B ON A.obdCode = B.obdCode and A.locationTime = B.lastTime"+
                " where A.longitude >=? and A.longitude <=? and A.latitude >=? and A.latitude <=? " ;

        System.out.println(sql);
    }

    @Override
    public void setDataSource(String jdbc, String user, String pwd) {
        this.jdbc = String.format("%s&user=%s&password=%s", jdbc, user, pwd);
    }

    /**
     * @param param 查询条件（未封装）
     * @return
     */
    @Override
    public List<IGeoMark> queryIMarks(Map<String, Object> param) throws Exception {
        Connection conn = DriverManager.getConnection(this.jdbc);
        PreparedStatement statement = conn.prepareStatement(this.sql);
        statement.setString(1, (String) param.get("westLon"));
        statement.setString(2, (String) param.get("eastLon"));
        statement.setString(3, (String) param.get("southLat"));
        statement.setString(4, (String) param.get("northLat"));

        ResultSet rs = statement.executeQuery();

        List<IGeoMark> listMarks = new ArrayList<>();
        while (rs.next()) {
            try {
                String obdCode = rs.getString("obdCode");
                Date tm = rs.getDate("lastTime");
                double lon = rs.getDouble("lon");
                double lat = rs.getDouble("lat");

                IGeoMark mark = new MyGeoMark(obdCode, tm, lon, lat);
                listMarks.add(mark);
            } catch (Exception ex) {
                // ignore
            }
        }

        conn.close();

        return listMarks;
    }

}
