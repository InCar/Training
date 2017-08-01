package com.incarcloud.core;/**
 * Created by fanbeibei on 2017/7/31.
 */

import java.util.List;
import java.util.Map;

/**
 * @author Fan Beibei
 * @Description: 数据库接口
 * @date 2017/7/31 13:58
 */
public interface IDataRepo<T extends IMark> {

    /**
     *
     * @param jdbc
     * @param user
     * @param pwd
     */
    void setDataSource(String jdbc, String user, String pwd);

    /**
     *
     * @param param 查询条件（未封装）
     * @return
     */
    List<T>  queryIMarks(Map<String,Object> param)throws Exception;


}
