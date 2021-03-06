# Сервер Redis

### Проект выполнил Гиркин Валерий Игоревич, студент группы БМИ143

### Постановка задачи:
Требовалось реализовать собственный сервер, реализующий часть протокола redis.

### Решенные задачи:
  1. Реализованы сериализатор и парсер для RedisValue (RedisNull, RedisInt, RedisString, RedisBulkString, RedisError, RedisArray), написаны тесты.
  2. Сервер принимает подключение от одного клиента и отвечает ошибкой на все запросы, также можно отвечать и самим запросом в виде списка строк (так, как его передал redis-cli).

### Используемые технологии:
  1. boost::variant.hpp - для парсера и сериализации.
  2. gtest - для тестирования.
  3. POSIX API - при написании сервера.
  4. ООП - отдельные классы для выполнения определенных задач.

### Как запустить:
  0. Установить оригинальный redis.
  1. Скомпилировать redis-server с помощью cmake и make.
  2. Запустить redis-server и redis-cli. (Возможно после запуска redis-cli придется убить процесс оригинального redis-server).
  3. В терминал с redis-cli писать запросы.
