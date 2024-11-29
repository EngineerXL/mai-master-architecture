import os
import sys
from sqlalchemy import create_engine, text
from sqlalchemy.orm import Session
from faker import Faker

SEED = 998244353

if __name__ == "__main__":
    login = os.getenv("DB_LOGIN")
    password = os.getenv("DB_PASSWORD")
    host = os.getenv("DB_HOST")
    db = os.getenv("DB_DATABASE")
    n_users = int(os.getenv("TEST_N_USERS"))
    fake = Faker(locale=["en_US"])
    fake.seed_instance(SEED)
    engine = create_engine(f"postgresql://{login}:{password}@{host}/{db}", echo=True)
    with Session(engine) as session:
        session.execute(text("DROP TABLE IF EXISTS users"))
        session.execute(
            text(
                """CREATE TABLE IF NOT EXISTS users (
                    id SERIAL PRIMARY KEY,
                    first_name VARCHAR(64) NOT NULL,
                    last_name VARCHAR(64) NOT NULL,
                    email VARCHAR(64),
                    login VARCHAR(64) UNIQUE NOT NULL,
                    password VARCHAR(64) NOT NULL
                );"""
            )
        )
        # todo --- store encrypted passwords
        for i in range(n_users):
            session.execute(
                text(
                    f"""INSERT INTO users(
                        first_name,
                        last_name,
                        email,
                        login,
                        password
                    ) VALUES (
                        '{fake.first_name()}',
                        '{fake.last_name()}',
                        '{fake.email()}',
                        '{fake.unique.user_name()}',
                        '{fake.password()}'
                    )"""
                )
            )
        session.commit()
    print(f"Database is successfully filled with {n_users} randomly generated user data.")