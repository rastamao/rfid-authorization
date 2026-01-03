<?php
// Подключение к базе данных MySQL
    // Параметры для подключения
    $db_host = "localhost"; 
    $db_user = ""; // Логин БД
    $db_password = ""; // Пароль БД
    $db_base = ''; // Имя БД
    $db_table = ""; // Имя Таблицы БД
try {
    $pdo = new PDO("mysql:host=$db_host;dbname=$db_base", $db_user, $db_password);
} catch (PDOException $e) {
    echo json_encode(["error" => "Не удалось подключиться к базе данных: " . $e->getMessage()]);
    exit;
}

header('Content-Type: application/json');

$id = trim(file_get_contents('php://input')); // читаем только ID

// Проверка наличия ID
if (empty($id)) {
    echo json_encode([
        "name_user" => null,
        "level_user" => null
    ]);
    exit;
}

$stmt = $pdo->prepare("SELECT name_user, level_user FROM users1 WHERE id = :id");
$stmt->bindParam(':id', $id);
$stmt->execute();

$user = $stmt->fetch();

if ($user) {
    echo json_encode([
        "name_user" => $user['name_user'],
        "level_user" => $user['level_user']
    ]);
} else {
    echo json_encode([
        "name_user" => null,
        "level_user" => null
    ]);
}
?>
