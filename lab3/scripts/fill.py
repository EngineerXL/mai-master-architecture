import os
import pymongo
import random
import sys
from faker import Faker

SEED = 998244353
TEST_TEXT_LEN = 64
TEST_MAX_CHAT_USERS = 24

if __name__ == "__main__":
    n_users = int(os.getenv("TEST_N_USERS"))
    n_chats = int(os.getenv("TEST_N_CHATS"))
    n_messages = int(os.getenv("TEST_N_MESSAGES"))
    max_chat_users = min(n_users, TEST_MAX_CHAT_USERS)
    fake = Faker(locale=["en_US"])
    fake.seed_instance(SEED)
    random.seed(SEED)
    
    client = pymongo.MongoClient(
        host=os.getenv("MONGO_HOST"),
        port=int(os.getenv("MONGO_PORT")),
        # username=os.getenv("MONGO_USER"),
        # password=os.getenv("MONGO_PASSWORD"),
    )
    db = client[os.getenv("MONGO_DATABASE")]
    chat_colletion = db["chats"]
    chat_colletion.drop()
    message_collection = db["messages"]
    message_collection.drop()
    for i in range(n_chats):
        users = sorted(random.sample(
            range(1, n_users + 1),
            random.choice([2, 2, 2, random.randint(3, max_chat_users)]),
        ))
        chat = {
            "_id": i + 1,
            "title": fake.text(TEST_TEXT_LEN),
            "users": users,
        }
        chat_colletion.insert_one(chat)
        for j in range(n_messages):
            message = {
                "_id": i * n_messages + j + 1,
                "chatid": i,
                "text": fake.text(TEST_TEXT_LEN),
                "user": random.choice(users),
            }
            message_collection.insert_one(message)
    print(
        f"Database is successfully filled with {n_chats} randomly generated chats, each with {n_messages} messages."
    )
