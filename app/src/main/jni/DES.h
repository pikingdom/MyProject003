 /******************************************************************
 **   PROGRAM: DES.h                                              **
 **   AUTHOR:  wenfj                                              **
 **   WRITE DATE:       06/18/2000                                **
 **   LAST MODIFY DATE: 06/18/2000                                **
 **   COMMENT: des头文件                                          **
 **                                                               **
 *******************************************************************/

#ifndef   _DES_H
#define   _DES_H

void DES(unsigned char *source,unsigned char *dest,unsigned char *key,unsigned char flag);

#ifndef   ENCRYPT
#define   ENCRYPT  0         /* DES 方向 */
#define   DECRYPT  1
#endif

#ifndef   STAND
#define   STAND    0         /* MAC 标准 */
#define   BPI      1
#endif

#endif
