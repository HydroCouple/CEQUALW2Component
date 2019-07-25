module test_add_module

     use iso_c_binding

     implicit none

     real(c_double), bind(C, name='test_add_module_a') :: a = 5.0

     contains
          real(c_double) function add(c) bind(C, name = 'test_add_module_add')
               real(c_double) :: c
               print *,  "a = ", a , "c =" , c
               a = a + c
               add = a
          end function add

end module test_add_module