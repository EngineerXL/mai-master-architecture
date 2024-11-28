workspace "Мессенджер" "Приложения для общения" {

    !identifiers hierarchical

    model {
        user = person "Пользователь"
        
        app = softwareSystem "Мессенджер \"FreeDurov\"" {
            description "Перспективный мессенджер на замену Telegram"

            user_service = container "Сервис пользователей" {
                description "Реализует взаимодействие пользователя с данными своего аккаунта"
            }

            message_service = container "Сервис сообщений" {
                description "Реализует общение пользователей в PtP чатах и группах"
            }

            group "Слой хранения" {
                user_db = container "База данных пользователей" {
                    description "Хранит данные о пользовательских аккаунтах"
                    tags "database"
                }

                chat_db = container "База данных PtP чатов, групп и сообщений" {
                    description "Хранит сообщения, данные о PtP чатах и группах"
                    tags "database"
                }
            }
        }

        user -> app "Общается"
        user -> app.user_service "Редактирует данные своего аккаунта"
        user -> app.message_service "Общается в чатах и группах"
        app.user_service -> app.user_db "Получение/обновление данных пользователей"
        app.message_service -> app.user_service "Получает информацию о пользователях"
        app.message_service -> app.chat_db "Получение/обновление сообщений, данных PtP чатов и групп"

        prod = deploymentEnvironment "Production" {
            deploymentGroup "Схема развёртывания"

            deploymentNode "Сервис пользователей" {
                containerInstance app.user_service
            }

            deploymentNode "Сервис сообщений" {
                containerInstance app.message_service
            }

            deploymentNode "База данных пользователей" {
                containerInstance app.user_db
                tags "database"
            }

            deploymentNode "База данных PtP чатов, групп и сообщений" {
                containerInstance app.chat_db
                tags "database"
            }
        }
    }

    views {
        themes default

        properties { 
            structurizr.tooltips true
        }
        
        systemContext app "Context" {
            include *
            autoLayout 
        }

        container app "Container" {
            include *
            autoLayout
        }

        deployment app prod {
            autoLayout
            include *
        }

        dynamic app "UC01" "Добавление нового пользователя" {
            autoLayout
            user -> app.user_service "Создать нового пользователя (POST /user)"
            app.user_service -> app.user_db "Сохранить данные о пользователе"
        }

        dynamic app "UC02" "Поиск пользователя по логину" {
            autoLayout
            user -> app.user_service "Найти пользователя (GET /user/search/login)"
            app.user_service -> app.user_db "Найти данные о пользователе"
        }

        dynamic app "UC03" "Поиск пользователя по имени или фамилии" {
            autoLayout
            user -> app.user_service "Найти пользователя (GET /user/search/name)"
            app.user_service -> app.user_db "Найти данные о пользователе"
        }

        dynamic app "UC04" "Создание группового чата" {
            autoLayout
            user -> app.message_service "Создать новую группу (POST /chat)"
            app.message_service -> app.chat_db "Сохранить данные о группе"
        }

        dynamic app "UC05" "Добавить пользователя в групповой чат" {
            autoLayout
            user -> app.message_service "Добавить пользователя в групповой чат (POST /chat/add)"
            app.message_service -> app.user_service "Найти пользователя (GET /user/search/name)"
            app.user_service -> app.user_db "Получить данные о пользователе"
            app.message_service -> app.chat_db "Обновить данные о группе"
        }

        dynamic app "UC06" "Добавление сообщения в групповой чат" {
            autoLayout
            user -> app.message_service "Написать сообщение в групповой чат (POST /chat/msg/write)"
            app.message_service -> app.chat_db "Найти групповой чат"
            app.message_service -> app.user_service "Найти пользователя (GET /user/search/login)"
            app.user_service -> app.user_db "Получить данные о пользователе"
            app.message_service -> app.user_service "Проверить, что пользователь состоит в группе"
            app.message_service -> app.chat_db "Сохранить сообщение"
        }

        dynamic app "UC07" "Загрузка сообщений группового чата" {
            autoLayout
            user -> app.message_service "Открыть групповой чат (GET /chat/msg/read)"
            app.message_service -> app.chat_db "Найти групповой чат"
            app.message_service -> app.user_service "Найти пользователя (GET /user/search/login)"
            app.user_service -> app.user_db "Получить данные о пользователе"
            app.message_service -> app.user_service "Проверить, что пользователь состоит в группе"
            app.message_service -> app.chat_db "Получить сообщения из группового чата"
            app.message_service -> app.user_service "Получить данные о пользователях (GET /user)"
            app.user_service -> app.user_db "Получить данные о пользователях"
        }

        dynamic app "UC08" "Отправка PtP сообщения пользователю" {
            autoLayout
            user -> app.message_service "Написать сообщение в PtP чат (POST /chat/msg/write)"
            app.message_service -> app.user_service "Найти пользователя (GET /user/search/name)"
            app.user_service -> app.user_db "Получить данные о пользователе"
            app.message_service -> app.chat_db "Сохранить сообщение"
        }

        dynamic app "UC09" "Получение списка PtP сообщения пользователю" {
            autoLayout
            user -> app.message_service "Открыть PtP чат (GET /chat/msg/read)"
            app.message_service -> app.user_service "Найти пользователя (GET /user/search/name)"
            app.user_service -> app.user_db "Получить данные о пользователе"
            app.message_service -> app.chat_db "Получить сообщения из PtP чата"
        }
        
        styles {
            element "database" {
                shape cylinder
            }
        }
    }
}
