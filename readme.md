# Restinio Backend
## Сервер, созданный по [заданию](https://app.swaggerhub.com/apis/Septemberer/yet-another_disk_open_api/1.0#/%D0%91%D0%B0%D0%B7%D0%BE%D0%B2%D1%8B%D0%B5%20%D0%B7%D0%B0%D0%B4%D0%B0%D1%87%D0%B8/post_imports) от Yandex.
### Для решения поставленных задач я решил использовать следующие библиотеки:
- #### [magic_enum](https://github.com/Neargye/magic_enum) Добавляет статическую рефлексию для перечислений. Использовал для преобразований типов элементов
- #### [Restinio](https://github.com/Stiffstream/restinio) Основа сервера. Обработка http запросов
- #### [Taopq](https://github.com/taocpp/taopq) Взаимодействие с БД(я выбрал Postgres). Помимо самих запросов, позволяет делать это безопасно, защищаясь от SQL инъекций
- #### [nlohmann_json](https://github.com/nlohmann/json) Обработка json нотации
### Билд системой выбран CMake в сочетании с пакетным менеджером Conan, что позволяет быстрее разворачивать проект без особых проблем
### Структура проекта:
| Путь | Описание |
| ------ | ------ |
| includes/answers/answers.h | Содержит стандартные ответы на запросы, такие как : валидация данных не прошла, ok, не найдено |
| include/handlers | Содержит обработчики запросов. Каждый требуемый запрос представлен отдельным обработчиком|
| include/items/SystemItem | Схема элемента, фукнкции для парсинга во что-то или из чего-то |
| include/server/server.h | Содержит функцию для создания [роутера](https://stiffstream.com/en/docs/restinio/0.6/expressrouter.html) для обработки запросовов|
| support/BDError | Перечисление ошибок базы данных в случае корректного запроса |

#### Установка на целевую машину:
```
git clone git@github.com:sabudilovskiy/RestInoBackend.git RestInioBackend
sudo bash install_deps_ubuntu.sh
```
##### Все зависимости будут скачаны и установлены. Остаётся только забилдить приложение