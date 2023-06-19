<?php
if ($_SERVER['REQUEST_METHOD'] == 'POST') {

    // save $_FILES['video'] to a folder
    $video = $_FILES['video'];
    $video_name = $video['name'];
    $video_tmp_name = $video['tmp_name'];
    $video_size = $video['size'];
    $video_error = $video['error'];

    $video_ext = explode('.', $video_name);
    $video_actual_ext = strtolower(end($video_ext));

    $allowed = array('mov', 'mp4', 'avi');

    if (in_array($video_actual_ext, $allowed)) {
        if ($video_error === 0) {
            if ($video_size < 10000000000000000000000) {
                $video_name_new = uniqid('', true) . '.' . $video_actual_ext;
                $video_destination = '../uploading/' . $video_name_new;
                move_uploaded_file($video_tmp_name, $video_destination);
                $_COOKIE['name'] = $_POST['name'];
                $_COOKIE['email'] = $_POST['email'];
                $_COOKIE['video'] = $video_destination;
                setcookie('name', $_POST['name'], time() + 3600 * 24 * 7);
                setcookie('email', $_POST['email'], time() + 3600 * 24 * 7);
                setcookie('video', $video_destination, time() + 3600 * 24 * 7);
            } else {
                echo 'File too big';
                exit(1);
            }
        } else {
            echo 'Error';
            exit(1);
        }
    } else {
        echo 'Not allowed';
        exit(1);
    }
} else if ($_SERVER['REQUEST_METHOD'] == 'GET' && isset($_GET['logout'])) {
    unset($_COOKIE['name']);
    unset($_COOKIE['email']);
    unset($_COOKIE['video']);
    setcookie('name', '', time() - 3600);
    setcookie('email', '', time() - 3600);
    setcookie('video', '', time() - 3600);
}
?>
<!DOCTYPE html>
<div>
    <?php if (isset($_COOKIE['name']) && isset($_COOKIE['email'])): ?>
        <p>
            <center><h1>Hello, <?= $_COOKIE['name'] ?>!</h1></center><hr>
            <center><video width="400" controls><source src="<?= $_COOKIE['video'] ?>" type="video/mp4"></video></center><hr><br/>
            <center><?php echo $_COOKIE['name']; ?></center>
            <center><?php echo $_COOKIE['email']; ?></center><br/><br/>
            <center><a href="?logout">Logout</a></center>
        </p>
    <?php else: ?>
        <form method="post" enctype="multipart/form-data">
            <input type="text" name="name" placeholder="Name" />
            <input type="text" name="email" placeholder="Email" />
            <input type="file" id="video" name="video" accept="video/mp4, video/avi, video/quicktime" />
            <input type="submit" value="Submit" />
        </form>
    <?php endif; ?>
</div>