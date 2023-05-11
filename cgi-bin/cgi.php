#!/usr/bin/php
<?php
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.php                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sriyani <sriyani@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:56:09 by sriyani           #+#    #+#             */
/*   Updated: 2023/04/13 11:56:12 by sriyani          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// $path_parts = pathinfo('/www/htdocs/inc/lib.inc.php');
    $path_parts = pathinfo('/home/USER/www.2021-05/');
    echo $path_parts['dirname'], "\n";
    echo $path_parts['basename'], "\n";
    echo $path_parts['extension'], "\n";
    echo $path_parts['filename'], "\n";
?>
