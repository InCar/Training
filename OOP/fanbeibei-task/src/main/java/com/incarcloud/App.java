package com.incarcloud;

import com.incarcloud.core.IBound;
import com.incarcloud.core.IDataRepo;
import com.incarcloud.core.IMark;
import com.incarcloud.core.IMarkContainer;
import com.incarcloud.my.geo.MyGeoBound;
import com.incarcloud.my.geo.MyGeoDataRepo;
import com.incarcloud.my.geo.MyGeoMarkContainer;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.*;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import java.util.List;

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


        IBound wuhan = new MyGeoBound(_wuhan.left, _wuhan.right, _wuhan.top, _wuhan.bottom);


        IDataRepo dataRepo = new MyGeoDataRepo();
        dataRepo.setDataSource(_cfg.DataBase, _cfg.User, _cfg.Password);
        IMarkContainer markContainer = new MyGeoMarkContainer(dataRepo);

        @SuppressWarnings("unchecked")
        List<IMark> markList = markContainer.getLastMarkInsideBound(wuhan);

        if(null != markList){
            for (IMark mark :markList) {
                System.out.println(String.format("%s is in %s at %s", mark.getId(), "wuhan", mark.getTimestamp()));
            }
        }

        System.out.println("=====end=====");
    }


}
