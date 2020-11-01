### chương trình kết nối ESP8266 tới MQTT Broker

### public free broker
- sử dụng trên ubuntu, docker
	docker pull emqx/emqx:4.2.2
	docker run -d --name emqx -p 1883:1883 -p 8083:8083 -p 8883:8883 -p 8084:8084 -p 18083:18083 emqx/emqx:4.2.2

### tài liệu tham khảo
- [thư viện PubSubClient](https://github.com/knolleary/pubsubclient/)
