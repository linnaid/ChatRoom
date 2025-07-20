all:write read

write::write.cc contacts.pb.cc
	g++ -o $@ $^ -std=c++17 -lprotobuf

read::read.cc contacts.pb.cc
	g++ -o $@ $^ -std=c++17 -lprotobuf

.PHONY:clean	
clean:
	rm -f write read