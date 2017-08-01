package com.incarcloud.my.geo;/**
 * Created by fanbeibei on 2017/7/31.
 */

import com.incarcloud.core.IDataRepo;
import com.incarcloud.core.IMark;
import com.incarcloud.core.geo.IGeoBound;
import com.incarcloud.core.geo.IGeoMark;
import com.incarcloud.core.geo.IGeoMarkContainer;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author Fan Beibei
 * @Description:
 * @date 2017/7/31 15:36
 */
public class MyGeoMarkContainer implements IGeoMarkContainer {
    private IDataRepo dataRepo;

    public MyGeoMarkContainer(IDataRepo dataRepo) {
        if (null == dataRepo) {
            throw new IllegalArgumentException();
        }

        this.dataRepo = dataRepo;
    }


    @Override
    public List<IGeoMark> getLastMarkInsideBound(IGeoBound bound) throws Exception {
        if (null == bound) {
            throw new IllegalArgumentException();
        }


        Map<String, Object> param = new HashMap<>(4);
        param.put("eastLon", "E"+bound.getEastLon());
        param.put("westLon", "E"+bound.getWestLon());
        param.put("northLat", "N"+bound.getNorthLat());
        param.put("southLat", "N"+bound.getSouthLat());

        @SuppressWarnings("unchecked")
        List<IGeoMark> markList = dataRepo.queryIMarks(param);
        if (null != markList && markList.size() > 0) {
            return markList;
        }

        return null;
    }
}
