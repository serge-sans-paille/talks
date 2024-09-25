TARGETS= \
		 most_vexing_parse \
		 raii \
		 print_vector \
		 convert_char_ptr_to_double \
		 copy_and_swap \
		 copy_elision \
		 initialization_list \
		 map_keys_to_vector \
		 option_type \
		 if_init \
		 if_init_unique \
		 tuple_as_lambda \
		 try_block_function \
		 non_throwing_swap \
		 named_parameters \
		 making_new_friends \
		 has_member \
		 has_member_function \
		 static_init_fiasco \
		 expression_templates \
		 local_class_definition \
		 execute_around_pointer \
		 \
		 named_params \
		 map_types \
		 filter_types \
		 curiously_recurring_template_pattern \
		 make_templated_class \
		 verbose_assert \
		 file_to_string \
		 print_line_of \
		 enable_if \
		 range_pair \
		 print_unpack \
		 multi_stmt_macro \
		 vertical_or_horizontal_storage \
		 sfinae \
		 named_params_2 \
		 lexicographical_compare_struct\

INDEX=index.rst


CLEANFILES=$(TARGETS) $(INDEX)
CXXFLAGS=-std=c++14 -Wall -O0 -g
all:$(TARGETS) $(INDEX)

clean:
	$(RM) $(CLEANFILES) $(INDEX) *.o

index.rst:$(TARGETS:%=%.cpp)
	for f in $^ ; do printf " - $$f\n" ; done > $@
