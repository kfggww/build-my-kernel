/* setup.s, 用于获取机器参数, 保存到原来的bootsect位置; 之后将system模块移动到0
 * 地址处; 配置好GDT, IDT, 打开A20地址线, 从而进入保护模式.
 */
