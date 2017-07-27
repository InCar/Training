package com.incarcloud;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.stereotype.Component;

@Component
class DatabaseConfig {
    @Value("${com.incarcloud.datasource.database}")
    String DataBase;

    @Value("${com.incarcloud.datasource.user}")
    String User;

    @Value("${com.incarcloud.datasource.pwd}")
    String Password;
}

@Component
class WuhanConfig{
    @Value("${com.incarcloud.wuhan.top}")
    double top;

    @Value("${com.incarcloud.wuhan.bottom}")
    double bottom;

    @Value("${com.incarcloud.wuhan.left}")
    double left;

    @Value("${com.incarcloud.wuhan.right}")
    double right;
}
