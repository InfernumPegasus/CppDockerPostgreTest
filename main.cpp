#include <pqxx/pqxx>
#include <iostream>

int main() {
    try {
        pqxx::connection conn(
                "dbname=postgres_db user=postgres_user password=postgres_password host=localhost port=5430");

        if (conn.is_open()) {
            std::cout << "Connected to database: " << conn.dbname() << std::endl;
        } else {
            std::cerr << "Failed to connect to database!" << std::endl;
            return 1;
        }

        pqxx::work txn(conn);
        pqxx::result res = txn.exec("SELECT version();");
        const auto version = res.front().front().c_str();
        std::cout << "PostgreSQL version: " << version << std::endl;

        txn.commit();
        conn.close();
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
