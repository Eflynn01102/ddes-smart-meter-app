package com.smartmeter.billing_service.security;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Profile;
import org.springframework.security.config.Customizer;
import org.springframework.security.config.annotation.web.builders.HttpSecurity;
import org.springframework.security.oauth2.jwt.JwtDecoder;
import org.springframework.security.oauth2.jwt.JwtDecoders;
import org.springframework.security.web.SecurityFilterChain;

@Configuration
@Profile("prod")
public class SecurityConfigProd {

  private static final Logger log = LoggerFactory.getLogger(SecurityConfigProd.class);

  @Bean
  SecurityFilterChain prodSecurity(HttpSecurity http) throws Exception {
    http.csrf(csrf -> csrf.disable());
    http.authorizeHttpRequests(reg -> reg
        .requestMatchers("/actuator/**").permitAll()
        .anyRequest().authenticated()
    );
    http.oauth2ResourceServer(oauth -> oauth.jwt(Customizer.withDefaults()));
    return http.build();
  }

  /**
   * Explicit JwtDecoder bean. Spring Boot should auto-configure this from issuer-uri, but
   * in constrained environments (e.g. no outbound network at startup) the auto-config may fail
   * silently and leave no bean. Providing it explicitly makes the failure clearer.
   */
  @Bean
  JwtDecoder jwtDecoder(@Value("${spring.security.oauth2.resourceserver.jwt.issuer-uri}") String issuer) {
    try {
      log.info("Configuring JwtDecoder from issuer: {}", issuer);
      return JwtDecoders.fromIssuerLocation(issuer);
    } catch (Exception e) {
      log.error("Failed to create JwtDecoder from issuer '{}'. If this environment blocks outbound calls, " +
          "consider supplying a jwk-set-uri or disabling auth temporarily.", issuer, e);
      throw e;
    }
  }
}
