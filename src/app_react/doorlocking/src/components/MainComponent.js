import React, { Component } from "react";
import { Route, Switch } from 'react-router-dom';
import Header from './HeaderComponent';
import Footer from './FooterComponent';
import Home from './HomeComponent';
import Menu from './MenuComponent';
import ListUsers from './ListUsersComponent';
import OpenDoor from './OpenDoorComponent';
import RegiserUser from './RegisterUserComponent';
import Logout from './LogoutComponent';

class Main extends Component {

    render(){
        return(
            <div id="layout">
                <Menu/>
                <div id="main">
                    <Header/>
                    <Switch>
                        <Route path="/" exact component={Home}/>
                        <Route path="/open_door" component={OpenDoor}/>
                        <Route path="/register_user" component={RegiserUser}/>
                        <Route path="/list_users" component={ListUsers}/>
                        <Route path="/logout_handler" component={Logout}/>
                    </Switch>
                    <Footer/>
                </div>
            </div>
        );
    }
}


export default Main;