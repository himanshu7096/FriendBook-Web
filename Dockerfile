FROM gcc:latest

WORKDIR /app

COPY . .

RUN g++ -std=c++17 src/server.cpp src/SocialNetwork.cpp -o friendbook_server

EXPOSE 10000

CMD ["./friendbook_server"]